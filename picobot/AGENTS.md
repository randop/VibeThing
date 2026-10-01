# Agent Guidance

## Project overview

`picobot` is IoT C++17 application targeting Luckfox Pico / RV1106 Linux on ARMv7-A. The current executable is built from `src/main.cpp`. Meson generates `config.h` from `config.in`; do not edit the generated header in the build directory.

## Architecture

### Overview

`picobot` is a Slack Socket Mode bot with integrated AI-agent support. It maintains a long-lived WebSocket to Slack's Socket Mode API and, for each incoming user message, optionally relays the prompt to an OpenAI-compatible AI endpoint, formatting the reply in Slack `mrkdwn`. Built-in commands (`/ping`, `/version`, `/use`, `/model`) are resolved locally without invoking the AI.

### Target platform and toolchain

- Deployment target: Luckfox Pico / Rockchip RV1106 Linux on ARMv7-A (little-endian, Cortex-A7, NEON-VFPv4 hard-float).
- Cross-compilation toolchain: `arm-rockchip830-linux-uclibcgnueabihf-` GCC/G++ from the Luckfox SDK, configured near the top of `luckfox-cross.ini`.
- C++17 is enforced both by Meson (`cpp_std=c++17` via `default_options`) and the cross file (`cpp_args` + `cpp_std = 'c++17'`). See `## Build` for the full toolchain/flag set.

### Build system and dependencies

- Build system: Meson (>=1.10). The executable target is declared in `meson.build` from `src/main.cpp`.
- Compile-time dependencies (all declared in `meson.build`):
  - **pthreads** (`dependency('threads')`) — backs the Slack-HTTP and AI-HTTP worker threads.
  - **Boost** (>=1.92.0, static) — Asio, Beast, filesystem, thread, URL, program_options, system, JSON, and context modules.
  - **wolfSSL** (>=5.9.2, static) — TLS backend for Boost.Asio.
- Release defaults: `optimization=3`, `b_lto=true`, `strip=true`, `b_ndebug=true`.
- TLS integration: `src/main.cpp` defines `BOOST_ASIO_USE_WOLFSSL`, `ASIO_USE_WOLFSSL`, and `OPENSSL_VERSION_NUMBER` *before* including wolfSSL/Boost.Asio headers. This define ordering must be preserved when editing TLS integration.

### Source layout and concurrency model

`src/main.cpp` is the single translation unit, structured as:

| Component | Responsibility |
|---|---|
| `https_client` | Coroutine-based HTTPS client (Asio + Beast + wolfSSL). Used for both Slack REST calls and AI-agent HTTP requests. |
| `SocketModeSession` | Manages the Slack Socket Mode WebSocket: connect, TLS handshake, heartbeat (ping/pong), forced reconnect, envelope parsing, ACK, and dispatching messages to built-in commands or the AI worker. Runs entirely on a Boost strand. |
| `main()` | Bootstraps three isolated `io_context`/`ssl::context` pairs and launches the session. |

Concurrency model:

- Three independent `asio::io_context` instances, each with its own `ssl::context` and `make_work_guard`:
  - **WebSocket** — run on the main thread (`wsIoc.run()`).
  - **Slack HTTP** — run on a dedicated thread (`httpIoc`).
  - **AI HTTP** — run on its own thread (`aiHttpIoc`), so AI latency cannot block the WebSocket loop.
- Coroutines via `asio::spawn` / `asio::yield_context` give linear, exception-driven control flow for async reads/writes/resolve.
- The WebSocket strand serialises session state (`aiBusy_`, timers, `ws_`); the AI worker executes on the AI HTTP executor and posts completion callbacks back to the strand before touching shared state.

### Configuration generation

- `VERSION` — authoritative project version, read at configure time via `run_command('cat', 'VERSION')` in `meson.build`.
- `config.in` — template defining `PROJECT_NAME`, `PROJECT_VERSION`, `BUILD_DATE`, and `APP_USER_AGENT`. Meson's `configure_file()` expands `@VAR@` placeholders into `config.h` inside the build directory. Edit `config.in` only; never edit the generated `config.h`.

### Formatting workflow

- `scripts/format.sh` runs `meson format -i` on `meson.build` and `clang-format -i` on `src/main.cpp`.
- New C++ files should be formatted with `clang-format` using the repo's `.clang-format` (LLVM-based: 2-space indent, 80-column limit, attached braces). See `## Formatting and changes`.

### Validation rules

- Configure and compile with `meson setup .build --buildtype=release --cross-file luckfox-cross.ini && meson compile -C .build`.
- Target-specific changes must be validated against the Luckfox cross build where the SDK and dependencies are available.
- Host-only builds are insufficient for target validation; no test suite is defined in `meson.build`. See `## Validation`.

### Key repository files

| File | Purpose |
|---|---|
| `meson.build` | Project metadata, compile options, dependency declarations, `config.h` generation, and executable target. |
| `VERSION` | Single version string consumed by `meson.build`. |
| `config.in` | Template for the generated `config.h`. |
| `src/main.cpp` | Application implementation (HTTPS client, WebSocket session, AI dispatch, `main`). |
| `luckfox-cross.ini` | ARM cross-compilation configuration and SDK prefixes. |
| `.clang-format` | clang-format style for C++ sources. |
| `scripts/format.sh` | Formatter wrapper (`meson format` + `clang-format`). |
| `README.md` | Setup/compile steps and device SSL certificate notes. |

## Build

The documented target build uses the Luckfox cross file:

```sh
meson setup .build --buildtype=release --cross-file luckfox-cross.ini
meson compile -C .build
```

The cross file expects the Luckfox ARM toolchain, Boost, and wolfSSL installations at the prefixes configured near the top of `luckfox-cross.ini`. Those paths are machine-specific; adjust them for the local SDK rather than hard-coding new developer paths in project code. Meson requires version 1.10 or newer. Dependencies are pthreads, static Boost (including filesystem, thread, URL, program_options, system, JSON, and context modules) version 1.92 or newer, and static wolfSSL version 5.9.2 or newer.

For a host build, use a separate build directory and omit `--cross-file`; host dependencies must still satisfy `meson.build`.

## Source and configuration

- Put application implementation in `src/`; keep the Meson target source list in `meson.build` up to date when adding files.
- Keep compile-time project metadata in `config.in`. Meson fills in version, project name, and build timestamp when configuring.
- `VERSION` is the authoritative project version and is read directly by `meson.build`.
- This target uses Boost.Asio/Beast and wolfSSL. Preserve the existing wolfSSL compatibility defines and include ordering when changing TLS integration; verify changes against the Luckfox toolchain and libraries.
- Avoid assumptions about desktop Linux or x86. The deployment target is little-endian ARMv7-A with Cortex-A7/NEON-VFPv4 hard-float settings.

## Formatting and changes

Run the repository formatter before committing C++ or Meson changes:

```sh
scripts/format.sh
```

It formats `meson.build` with `meson format` and `src/main.cpp` with `clang-format`. Format additional C++ files with `clang-format` as they are added. Keep changes focused and follow the surrounding C++ style and the repository's `.clang-format` configuration.

## Validation

After source or build-definition changes, configure or reconfigure the appropriate Meson build directory and compile it. For target-specific changes, use the Luckfox cross build where the SDK and dependencies are available. Do not claim target validation from a host-only build. There is no separate test suite currently defined in `meson.build`.

## Repository map

- `meson.build`: project settings, dependency declarations, generated header, and executable target.
- `luckfox-cross.ini`: ARM cross-compilation configuration and SDK prefixes.
- `src/main.cpp`: current application implementation.
- `config.in`: generated build configuration template.
- `scripts/format.sh`: formatting helper.
- `README.md`: setup, compile, and device certificate notes.
- `VERSION`: version used by Meson.
