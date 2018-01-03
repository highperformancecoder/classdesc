
project('classdesc', 'cpp',
    version: '3.34',
    meson_version: '>=0.39.1',
    license: [ 'GPL3+', 'MIT' ],
    default_options: [
        'cpp_std=c++14',
        'warning_level=1',
    ],
)

sources = ['classdesc.cc',  
           'insert-friend.cc',  
           'xdr_pack.cc'
           ]
includes = []
extra_libs = []
incdir = include_directories('include')
executable('classdesc', 'classdesc.cc', include_directories : incdir)
executable('insert-friend', 'insert-friend.cc', include_directories : incdir)



