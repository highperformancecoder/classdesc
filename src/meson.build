subdir('test')
add_global_arguments('-DXDR_PACK', '-DHAVE_LONGLONG', language : 'cpp')
classdesc = executable('classdesc', 'classdesc.cc', include_directories : incdir)
executable('insert-friend', 'insert-friend.cc', include_directories : incdir)
#if get_option('enable-XDR') sources += 'xdr_pack.cc' endif
#if get_option('enable-XDR') cflags += '-DXDR_PACK, '-DTR1' endif

