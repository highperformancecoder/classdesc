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
assert root.bar.a._signature==[{'args': [], 'ret': 'double'}, {'args': ['double'], 'ret': 'double'}]
assert root.bar.a._type=='double'
assert root.bar.a._list==[]

assert near(root.bar.af(),0.2)
assert near(root.bar.af(0.3),0.3)
assert near(root.bar.af(),0.3)
assert root.bar.af._signature==[{'args': [], 'ret': 'float'}, {'args': ['float'], 'ret': 'float'}]

assert root.bar.b()==3
assert root.bar.b(1)==1
assert root.bar.b()==1
assert root.bar.b._signature==[{'args': [], 'ret': 'int'}, {'args': ['int'], 'ret': 'int'}]

assert root.bar.barE()=='a'
assert root.bar.barE._signature==[{'args': ['::Bar::BarE'], 'ret': '::Bar::BarE'}, {'args': [], 'ret': '::Bar::BarE'}]

assert root.bar.bf()==False
assert root.bar.bf(True)==True
assert root.bar.bf()==True
assert root.bar.bf._signature==[{'args': [], 'ret': 'bool'}, {'args': ['bool'], 'ret': 'bool'}]

assert root.bar.bt()==True
assert root.bar.bt(False)==False
assert root.bar.bt()==False

assert root.bar.c()=='\r hello & 123 '
assert root.bar.c('hello world')=='hello world'
assert root.bar.c()=='hello world'

assert root.bar.c1()._properties==['\r','\r']
assert root.bar.c1[0]=='\r'
root.bar.c1[0]='x'
assert root.bar.c1[0]=='x'

assert root.bar.ef()=='ea'
assert root.bar.ef('eb')=='eb'
assert root.bar.ef()=='eb'
# setting an enum to a nonsense value sets it to the default
assert root.bar.ef('foo')=='ea'
assert root.bar.ef()=='ea'
assert root.bar.ef._type=='::EnumFoo'
assert root.bar.ef._list==[]

assert root.bar.d()._properties==[0,1,2]
assert len(root.bar.d)==3
assert root.bar.d[1]==1
root.bar.d[1]=5
assert root.bar.d[1]==5

assert len(root.bar.d1)==2
assert root.bar.d1[1]._properties==[3,4,5]
root.bar.d1[1]=[6,7,8]
assert root.bar.d1[1]._properties==[6,7,8]
assert root.bar.d1()._properties==[[0,1,2],[6,7,8]]
root.bar.d1[1][2]=10
assert root.bar.d1[1][2]==10
assert root.bar.h()._properties==[2,2,2]
assert root.bar.h[1]==2
root.bar.h[1]=3
assert root.bar.h[1]==3
assert root.bar.h._insert(5)._properties==[2,3,2,5]
assert root.bar.h()._properties==[2,3,2,5]
assert root.bar.h._erase(1)._properties==[2,2,5]
assert root.bar.h()._properties==[2,2,5]
assert root.bar.h([2,2,2,2])._properties==[2,2,2,2]
assert root.bar.h._type=="std::vector<int>"
assert root.bar.h._list==[".@elem",".@elemNoThrow",".@insert",".@erase",".@size"]

assert root.bar.l()._properties==[2,2,2]
assert root.bar.l[1]==2
root.bar.l[1]=3
assert root.bar.l[1]==3
assert root.bar.l._insert(5)._properties==[2,3,2,5]
assert root.bar.l()._properties==[2,3,2,5]
assert root.bar.l([2,2,2,2])._properties==[2,2,2,2]

assert root.bar.m()._properties==[{"first":0,"second":5},{"first":3,"second":2}]
assert len(root.bar.m)==2
assert root.bar.m._signature==[{"args":[],"ret":"std::map<int,int>"},{"args":["std::map<int,int>"],"ret":"std::map<int,int>"}]
assert root.bar.m[3]==2
root.bar.m[3]=5
assert root.bar.m[3]==5

# should throw
thrown=False
try:
    root.bar.m[1]
except:
    thrown=True
assert thrown

assert root.bar.m._insert({"first":1,"second":2})._properties==[{"first":0,"second":5},{"first":1,"second":2},{"first":3,"second":5}]
assert root.bar.m[1]==2
assert root.bar.m._type=="std::map<int,int>"
assert root.bar.m._list==[".@elem",".@elemNoThrow",".@insert",".@erase",".@size",".@keys"]
assert root.bar.m._erase(1)._properties==[{"first":0,"second":5},{"first":3,"second":5}]
assert root.bar.m()._properties==[{"first":0,"second":5},{"first":3,"second":5}]
assert root.bar.m._keys()._properties==[0,3]

assert root.bar.ss(["hello","foo","bar"])._properties==['bar', 'foo', 'hello']
assert root.bar.ss()._properties==['bar', 'foo', 'hello']

assert root.bar.um._insert({"first":1,"second":3})._properties==[{"first":1,"second":3}]
assert root.bar.um()._properties==[{"first":1,"second":3}]
root.bar.um[1]=5
assert root.bar.um[1]==5

assert root.bar.llex()._properties==[["hello","hello"],["hello","hello"]]
root.bar.llex[1]=["baa","baa","blacksheep"]
assert root.bar.llex[1]._properties==["baa","baa","blacksheep"]
assert root.bar.llex()._properties==[["hello","hello"],["baa","baa","blacksheep"]]
assert root.bar.llex._signature==[{"args":[],"ret":"std::list<std::list<std::string>>"},{"args":["std::list<std::list<std::string>>"],"ret":"std::list<std::list<std::string>>"}]
assert root.bar.llex[1][1]=="baa"

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
