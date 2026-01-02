project('blink-stm32f103', 'c',
  version : '0.1',
  license : 'MIT',
  default_options : ['warning_level=3', 'buildtype=debugoptimized']
)

if not meson.is_cross_build()
  error('This is an embedded project → must use cross build!')
endif

add_global_arguments('-DSTM32F103xB', language : 'c')

objcopy = find_program('arm-none-eabi-objcopy')
size    = find_program('arm-none-eabi-size')

sources = files(
  'src/main.c',
  'startup/startup_stm32f103xb.s'
)

linker_script = meson.current_source_dir() / 'linker/STM32F103C8Tx_FLASH.ld'

inc_dirs = include_directories('src')

elf = executable('blink.elf',
  sources : sources,
  include_directories : inc_dirs,
  link_args : ['-T', linker_script],
  name_suffix : '.elf'
)

# ─────────────────────────────────────────────────────────────────────────────
# Fixed custom_target: now explicitly declares the ELF as input
# ─────────────────────────────────────────────────────────────────────────────

bin = custom_target('blink.bin',
  input : elf,
  output : 'blink.bin',
  command : [objcopy, '-O', 'binary', '@INPUT@', '@OUTPUT@'],
  build_by_default : true
)

run_target('size',
  command : [size, '--format=berkeley', elf],
  depends : elf
)

run_target('flash',
  command : [
    'openocd', '-f', 'interface/stlink.cfg',
    '-c', 'set CPUTAPID 0',  # ← important for fake/clone Blue Pills
    '-f', 'target/stm32f1x.cfg',
    '-c', 'program ' + bin.full_path() + ' verify reset exit 0x08000000'
  ],
  depends : bin
)
