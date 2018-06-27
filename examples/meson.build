exes=['heat', 'graphnode', 'ptrflag', 'refnode', 'old-polymorph',
      'polymorph2', 'internal_type', 'polymorph']

cdhdrs=['polymorph','polymorph2','graphnode','arr2D','internal_type',
        'old-polymorph','ptrflag','refnode']

descriptors=['pack', 'unpack',  'xml_pack', 'xml_unpack']

classdescCmd=[classdesc,'-typeName','-use_mbr_pointers',
                '-i','@INPUT@']
foreach d: descriptors
  classdescCmd+=d
endforeach   

sources+='polymorph.cc'

foreach c: cdhdrs
  custom_target(c+'.cd',output:c+'.cd',input:c+'.h',
        command:classdescCmd,capture:true,build_by_default:true)
endforeach

foreach e: exes
  executable(e, e+'.cc', include_directories: incdir, link_with: libclassdesc)
endforeach   

