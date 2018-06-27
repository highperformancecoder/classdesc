
project('classdesc', 'cpp',
    version: '3.34',
    meson_version: '>=0.39.1',
    license: [ 'GPL3+', 'MIT' ],
    default_options: [
        'cpp_std=c++03',
        'warning_level=3',
        'prefix=/usr',
    ],
)

sources = []

add_global_arguments('-DHAVE_LONGLONG', language : 'cpp')
add_global_arguments('-g -I.. -I. -I/usr/X11R6/include -DRECUR_MAX=50 -DTR1', 
                         language: 'c++')

includes = []
extra_libs = []
incdir = include_directories('include')
subdir('src')
subdir('include')
subdir('lib')
subdir('examples')



