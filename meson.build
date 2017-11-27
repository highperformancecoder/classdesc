
project('classdesc', 'cpp',
    version: '3.34',
    meson_version: '>=0.39.1',
    license: [ 'GPL3+', 'MIT' ],
    default_options: [
        'cpp_std=c++14',
        'warning_level=1',
        'prefix=/usr',
    ],
)

sources = []

add_global_arguments('-DXDR_PACK', '-DHAVE_LONGLONG', language : 'cpp')
includes = []
extra_libs = []
srcdir = include_directories('src')
incdir = include_directories('src/include')
testdir = include_directories('src/test')
inctestdir = include_directories('src/test/include')
subdir('src')





