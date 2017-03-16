macos_flags = [
  '-DWITH_COREAUDIO',
]

macos_sources = glob([
  'src/backend/coreaudio/**/*.cpp',
])

platform_sources = macos_sources

macos_linker_flags = [
  '-framework', 'AudioToolbox',
  '-framework', 'CoreAudio',
]

cxx_library(
  name = 'soloud',
  header_namespace = '',
  exported_headers = subdir_glob([
    ('include', '*.h'),
  ]),
  headers = subdir_glob([
    ('src', '**/*.h'),
  ]),
  srcs = glob([
    'src/audiosource/**/*.cpp',
    'src/audiosource/**/*.c',
    'src/backend/null/**/*.cpp',
    'src/c_api/**/*.cpp',
    'src/core/**/*.cpp',
    'src/filter/**/*.cpp',
  ],
  excludes = platform_sources),
  platform_srcs = [
    ('default', macos_sources),
    ('^macos.*', macos_sources),
  ],
  compiler_flags = [
    '-DWITH_NULL',
  ],
  platform_compiler_flags = [
    ('default', macos_flags),
    ('^macos.*', macos_flags),
  ],
  platform_linker_flags = [
    ('default', macos_linker_flags),
    ('^macos.*', macos_linker_flags),
  ],
  exported_platform_linker_flags = [
    ('default', macos_linker_flags),
    ('^macos.*', macos_linker_flags),
  ],
  visibility = [
    'PUBLIC',
  ],
)
