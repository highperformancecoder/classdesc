
project('classdesc', 'cpp',
    version: '3.34',
    meson_version: '>=0.39.1',
    license: [ 'GPL3+', 'MIT' ],
    default_options: [
        'cpp_std=c++14',
        'warning_level=1',
    ],
)

sources = ['xdr_pack.cc'
           ]
add_global_arguments('-DXDR_PACK', '-DHAVE_LONGLONG', language : 'cpp')
includes = []
extra_libs = []
incdir = include_directories('src/include')
executable('classdesc', 'classdesc.cc', include_directories : incdir)
executable('insert-friend', 'insert-friend.cc', include_directories : incdir)



