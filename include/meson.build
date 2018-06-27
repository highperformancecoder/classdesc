custom_target('unpack_base.h',output:'unpack_base.h',input:'pack_base.h',
        command:['cp','@INPUT@','@OUTPUT@'])

# header files to be processed by classdesc
cdhdrs=['object','polyBase']
# descriptors to generate
descriptors=['pack', 'unpack',  'xml_pack', 'xml_unpack']

classdescCmd=[classdesc,'-typeName','-use_mbr_pointers',
                '-i','@INPUT@']
foreach d: descriptors
  classdescCmd+=d
endforeach   

foreach c: cdhdrs
  custom_target(c+'.cd',output:c+'.cd',input:c+'.h',
        command:classdescCmd,capture:true,build_by_default:true)
endforeach

bash=find_program('bash',required: false)
if bash.found()
  custom_target('functiondb.h', output:'functiondb.h', input: 'functiondb.sh',
    command:['bash', '@INPUT@'], capture:true,build_by_default:true)
endif
