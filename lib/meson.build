sources=['xdr_pack.cc']
libs = []
message(host_machine.system())
tirpc=dependency('libtirpc', required:false)
if tirpc.found()
   libs+='-ltirpc'
endif     
libclassdesc = library('classdesc', sources, include_directories :
incdir, link_args: libs)
