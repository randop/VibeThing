project('Things Interservice', ['cpp', 'c'],
  version : '0.1',
  default_options : ['cpp_std=c++20', 'c_std=c11']
)

libuv_dep = dependency('libuv')
wolfssl_dep = dependency('wolfssl')

executable('iotsvc', ['main.cpp', 'vendor/picohttpparser.c'],
  dependencies : [libuv_dep, wolfssl_dep],
  include_directories : include_directories('vendor')
)