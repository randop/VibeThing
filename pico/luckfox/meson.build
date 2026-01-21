project('luckfox', 'cpp',
  version : run_command('cat', 'VERSION', check: true).stdout().strip(),
  meson_version : '>=1.10.0',
  default_options : ['cpp_std=c++17']
)

# Capture build date/time
# Format example: "2026-01-21 21:45:00 UTC"
build_date_cmd = run_command('date', '+%Y-%m-%d %H:%M:%S UTC', check: true)
build_date = build_date_cmd.stdout().strip()

conf_data = configuration_data()
conf_data.set('PROJECT_VERSION', meson.project_version())
conf_data.set('PROJECT_NAME', meson.project_name())
conf_data.set('BUILD_DATE', build_date)
configure_file(
  input : 'config.in',
  output : 'config.h',
  configuration : conf_data
)

includes = include_directories('.')

executable('luckfox',
  'src/main.cpp',
  include_directories: includes
)
