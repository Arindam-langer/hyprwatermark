project('hypr-watermark', 'cpp',
  version: '0.1.0',
  default_options: ['buildtype=release'],
)

cpp_compiler = meson.get_compiler('cpp')
if cpp_compiler.has_argument('-std=c++23')
  add_global_arguments('-std=c++23', language: 'cpp')
elif cpp_compiler.has_argument('-std=c++2b')
  add_global_arguments('-std=c++2b', language: 'cpp')
else
  error('C++23 support required')
endif

hyprland = dependency('hyprland', required: true)

shared_module('hypr-watermark',
  'src/main.cpp',
  'src/WatermarkDecoration.cpp',
  dependencies: [hyprland],
  install: true,
)
