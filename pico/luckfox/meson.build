project(
    'luckfox',
    'cpp',
    version: run_command('cat', 'VERSION', check: true).stdout().strip(),
    meson_version: '>=1.10.0',
    default_options: [
        'cpp_std=c++17',
        'optimization=3',
        'strip=true',
        'b_lto=true',
        'b_ndebug=true',
    ],
)

# Capture build date/time
# Format example: "2026-01-21 21:45:00 UTC"
build_date_cmd = run_command('date', '+%Y-%m-%d %H:%M:%S UTC', check: true)
build_date = build_date_cmd.stdout().strip()

conf_data = configuration_data()
conf_data.set('PROJECT_VERSION', meson.project_version())
conf_data.set('PROJECT_NAME', meson.project_name())
conf_data.set('BUILD_DATE', build_date)
configure_file(input: 'config.in', output: 'config.h', configuration: conf_data)

###############################################################################
# Dependencies
###############################################################################

thread_dep = dependency('threads')

boost_dep = dependency(
    'boost',
    version: '>=1.92.0',
    modules: [
        'filesystem',
        'thread',
        'url',
        'program_options',
        'system',
        'json',
    ],
    required: true,
    static: true,
)

wolfssl_dep = dependency(
    'wolfssl',
    version: '>=5.9.2',
    required: true,
    static: true,
)

includes = include_directories('.')

executable(
    'lcfx',
    'src/main.cpp',
    include_directories: includes,
    dependencies: [thread_dep, boost_dep, wolfssl_dep],
)
