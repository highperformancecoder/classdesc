from pyExample import root

def near(x,y):
    return abs(x-y)/(abs(x)+abs(y))<1e-4
    
root()
assert root.bar.csi()==20

# should throw
thrown=False
try:
    root.bar.csi(100)
except:
    thrown=True
assert thrown

assert root.bar.si()==0
assert root.bar.si(10)==10
assert root.bar.si()==10
assert root.bar1.f.si()==10
assert root.bar.ch()=='M'

assert near(root.bar.a(),0.1)
assert near(root.bar.a(0.2),0.2)
assert near(root.bar.a(),0.2)
assert root.bar.a._signature()==[{'args': [], 'ret': 'double'}, {'args': ['double'], 'ret': 'double'}]
assert root.bar.a._type()=='double'
assert root.bar.a._list==[]

assert near(root.bar.af(),0.2)
assert near(root.bar.af(0.3),0.3)
assert near(root.bar.af(),0.3)
assert root.bar.af._signature()==[{'args': [], 'ret': 'float'}, {'args': ['float'], 'ret': 'float'}]

assert root.bar.b()==3
assert root.bar.b(1)==1
assert root.bar.b()==1
assert root.bar.b._signature()==[{'args': [], 'ret': 'int'}, {'args': ['int'], 'ret': 'int'}]

assert root.bar.barE()=='a'
assert root.bar.barE._signature()==[{'args': ['::Bar::BarE'], 'ret': '::Bar::BarE'}, {'args': [], 'ret': '::Bar::BarE'}]

assert root.bar.bf()==False
assert root.bar.bf(True)==True
assert root.bar.bf()==True
assert root.bar.bf._signature()==[{'args': [], 'ret': 'bool'}, {'args': ['bool'], 'ret': 'bool'}]

assert root.bar.bt()==True
assert root.bar.bt(False)==False
assert root.bar.bt()==False

assert root.bar.c()=='\r hello & 123 '
assert root.bar.c('hello world')=='hello world'
assert root.bar.c()=='hello world'

assert root.bar.c1()==['\r','\r']
#
#/root/bar/c1/@elem/0
#
#/root/bar/c1/@elem/0
#"x"
#/root/bar/c1
#
#/root/bar/ef
#
#/root/bar/ef
#"eb"
#/root/bar/ef
#
#/root/bar/ef
#"foo"
#/root/bar/ef
#
#/root/bar/ef/@type
#
#/root/bar/ef/@list
#
#/root/bar/d
#
#/root/bar/d/@size
#
#/root/bar/d/@elem/1
#
#/root/bar/d/@elem/1
#5
#/root/bar/d
#
assert len(root.bar.d1)==2
assert root.bar.d1[1]==[3,4,5]
root.bar.d1[1]=[6,7,8]
assert root.bar.d1[1]==[6,7,8]
#/root/bar/d1
#
#/root/bar/d1/@elem/1/@elem/2
#10
#/root/bar/h
#
#/root/bar/h/@elem/1
#
#/root/bar/h/@elem/1
#3
#/root/bar/h
#
#/root/bar/h/@insert
#5
#/root/bar/h
#
#/root/bar/h/@erase
#1
#/root/bar/h
#
#/root/bar/h
#[2,2,2,2]
#/root/bar/h/@type
#
#/root/bar/h/@list
#
#/root/bar/l
#
#/root/bar/l/@elem/1
#
#/root/bar/l/@elem/1
#3
#/root/bar/l
#
#/root/bar/l/@insert
#5
#/root/bar/l
#
#/root/bar/l
#[2,2,2,2]
#/root/bar/m   
#
#/root/bar/m/@size   
#
#/root/bar/m/@signature
#
#/root/bar/m/@elem/3
#
#/root/bar/m/@elem/3
#5
#/root/bar/m/@elem/3
#
## should throw
#/root/bar/m/@elem/1
#
#/root/bar/m/@insert
#{"first":1,"second":2}
#/root/bar/m/@elem/1
#
#/root/bar/m/@type
#
#/root/bar/m/@list
#
#/root/bar/m/@erase
#1
#/root/bar/m/
#
#/root/bar/m/@keys
#
#/root/bar/ss
#["hello","foo","bar"]
#/root/bar/ss
#
#/root/bar/um/@insert
#{"first":1,"second":3}
#/root/bar/um
#
#/root/bar/um/@elem/1
#5
#/root/bar/um/@elem/1
#
#/root/bar/llex
#
#/root/bar/llex/@elem/1
#["baa","baa","blacksheep"]
#/root/bar/llex
#
#/root/bar/llex/@signature
#
#/root/bar/llex/@elem/1/@elem/1
#
## Should fail
#/root/bar/iex
#
#/root/bar/sef
#
#/root/bar/sef
#"eb"
#/root/bar/sef
#
#/root/bar/vs0/@signature
#
#/root/bar/vs0/
#
#/root/bar/shello
#
#/root/bar/seqLength
#[[1,2,3]]
#/root/bar/name
#
#/root/bar/overloadExample
#
#/root/bar/barE
#
#/root/bar/barE 
#"b"
#/root/bar/globE
#
#/root/bar1/foo
#
#/root/bar1/fooRef/b
#
#/root/bar1/f/name
#
#/root/bar1/vFoo
#
#/root/bar1/vFoo/@elem/1
#
#/root/bar1/vFoo/@elem/10
#
#/root/bar1/foo/b
#3
#/root/bar1/foo/b
#
#/root/bar1/fooRef/b
#3
#/root/bar1/fooRef/b
#
#/root/bar1/foo/b
#
#/root/bar1/foo/
#
#/root/bar1/foo/
#
#/root/bar1/sfoop
#
#/root/bar1/recursiveType
#"hello"
#/root/defaultless/foo
#
#/root/defaultless/bar
#
#/root/getFB1
#
#/root/getFB1/f/b
#
## enum enumerator listing
#/@enum/@list
#
#/@enum/::EnumFoo
#
#/@enum/::GlobE
#
#/@enum/::Bar::BarE
#
## StringKeyMap
#/root/bar/sm
#
#/root/bar/sm
#{"foo":1,"bar":2}
#/root/bar/sm
#
#/root/bar/sm
#{"foo":1,"foobar":3}
#/root/bar/sm/@elem/foo
#
#/root/bar/sm/@erase
#"foobar"
#/root/bar/sm/@insert
#{"first":"bar","second":1}
#/root/bar/sm/@keys
#
#/root/bar/rotation
#
#/root/bar/rotation
#20
#/root/bar/rotation
#
#/root/testString
#"hello"
#
#/root/testDoubleIntOverload
#1
#/root/testDoubleIntOverload
#[1,1]
#/root/testDoubleIntOverload
#[1,1.1]
#/root/testDoubleIntOverload
#[1.1,1]
#/root/dummy
#{a:1}
