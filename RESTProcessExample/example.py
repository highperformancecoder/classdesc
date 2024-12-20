from pyExample import root,enum

def near(x,y):
    return abs(x-y)/(abs(x)+abs(y))<1e-4

def expectThrow(f):
    thrown=False
    try:
        f()
    except:
        thrown=True
    assert thrown

assert root._type()=='::Root'
assert root.bar.csi()==20

# should throw
expectThrow(lambda: root.bar.csi(100))

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
assert root.bar.a._list()==[]

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
assert root.bar.c1[0]=='\r'
root.bar.c1[0]='x'
assert root.bar.c1[0]=='x'

assert root.bar.ef()=='ea'
assert root.bar.ef('eb')=='eb'
assert root.bar.ef()=='eb'
# setting an enum to a nonsense value sets it to the default
assert root.bar.ef('foo')=='ea'
assert root.bar.ef()=='ea'
assert root.bar.ef._type()=='::EnumFoo'
assert root.bar.ef._list()==[]

assert root.bar.d()==[0,1,2]
assert len(root.bar.d)==3
assert root.bar.d[1]==1
root.bar.d[1]=5
assert root.bar.d[1]==5

assert len(root.bar.d1)==2
assert root.bar.d1[1]()==[3,4,5]
root.bar.d1[1]=[6,7,8]
assert root.bar.d1[1]()==[6,7,8]
assert root.bar.d1()==[[0,1,2],[6,7,8]]
root.bar.d1[1][2]=10
assert root.bar.d1[1][2]==10
assert root.bar.h()==[2,2,2]
assert root.bar.h[1]==2
root.bar.h[1]=3
assert root.bar.h[1]==3
root.bar.h.insert(5)
assert root.bar.h()==[2,3,2,5]
root.bar.h.erase(1)
assert root.bar.h()==[2,2,5]
assert root.bar.h([2,2,2,2])()==[2,2,2,2]
assert root.bar.h._type()=="std::vector<int>"
assert root.bar.h._list()==[".@elem",".@elemNoThrow",".@erase",".@insert",".@size"]

assert root.bar.l()==[2,2,2]
assert root.bar.l[1]==2
root.bar.l[1]=3
assert root.bar.l[1]==3
root.bar.l.insert(5)
assert root.bar.l()==[2,3,2,5]
assert root.bar.l([2,2,2,2])()==[2,2,2,2]

assert root.bar.m()==[{"first":0,"second":5},{"first":3,"second":2}]
assert len(root.bar.m)==2
assert root.bar.m._signature()==[{"args":[],"ret":"std::map<int,int>"},{"args":["std::map<int,int>"],"ret":"std::map<int,int>"}]
assert root.bar.m[3]==2
root.bar.m[3]=5
assert root.bar.m[3]==5

root.bar.m.insert({"first":1,"second":2})
assert root.bar.m[1]==2
assert root.bar.m.contains(1)
root.bar.m[10]=2
assert root.bar.m[10]==2
assert root.bar.m.contains(10)
assert root.bar.m._type()=="std::map<int,int>"
assert root.bar.m._list()==[".@elem",".@elemNoThrow",".@erase",".@insert",".@keys",".@size"]
root.bar.m.erase(1)
root.bar.m.erase(10)
assert root.bar.m()==[{"first":0,"second":5},{"first":3,"second":5}]
assert root.bar.m.keys()==[0,3]

assert root.bar.ss(["hello","foo","bar"])()==['bar', 'foo', 'hello']
assert root.bar.ss()==['bar', 'foo', 'hello']

root.bar.um.insert({"first":1,"second":3})
assert root.bar.um()==[{"first":1,"second":3}]
root.bar.um[1]=5
assert root.bar.um[1]==5

assert root.bar.llex()==[["hello","hello"],["hello","hello"]]
root.bar.llex[1]=["baa","baa","blacksheep"]
assert root.bar.llex[1]()==["baa","baa","blacksheep"]
assert root.bar.llex()==[["hello","hello"],["baa","baa","blacksheep"]]
assert root.bar.llex._signature()==[{"args":[],"ret":"std::list<std::list<std::string>>"},{"args":["std::list<std::list<std::string>>"],"ret":"std::list<std::list<std::string>>"}]
assert root.bar.llex[1][1]=="baa"

# Should fail
expectThrow(lambda: root.bar.iex())

assert root.bar.sef()=='ea'
assert root.bar.sef('eb')=='eb'
assert root.bar.sef()=='eb'

assert root.bar.vs0._signature()==[{"args":[],"ret":"std::string"}]
assert root.bar.vs0()==' hello'
assert root.bar.shello()=='hello'
assert root.bar.seqLength([1,2,3])==3
assert root.bar.name()=='Bar'
assert root.bar.overloadExample(2,3)==5
assert root.bar.overloadExample().a()==0.2

assert root.bar.barE()=='a'
assert root.bar.barE('b')=='b'
assert root.bar.globE()=='ga'

assert root.bar1.foo().a()==0.1
assert root.bar1.fooRef().b()==2
assert root.bar1.f.name()=='Foo'

assert root.bar1.vFoo[1]()['b']==1
assert root.bar1.vFoo[1].b()==1
expectThrow(lambda: root.bar1.vFoo[10])

assert root.bar.mFoo['foo'].b()==2

assert root.bar1.foo().b(3)==3
assert root.bar1.foo().b()==2
assert root.bar1.fooRef().b(3)==3
assert root.bar1.fooRef().b()==3
assert root.bar1.foo().b()==3

assert root.bar1.sfoop()==None

assert root.bar1.recursiveType('hello')._properties()['f']['b']==3

assert root.defaultless.foo()==0
assert root.defaultless.bar()==2

root.getFB1()
assert root.getFB1().f.b()==0

# enum enumerator listing
assert list(enum.keys())==['::Bar::BarE', '::EnumFoo', '::GlobE']
assert enum['::EnumFoo']==['ea', 'eb', 'ec', 'ed']
assert enum['::GlobE']==['ga', 'gb']
assert enum['::Bar::BarE']==['a', 'b']

# StringKeyMap
assert root.bar.sm()=={}
assert root.bar.sm({"foo":1,"bar":2})._properties()=={"foo":1,"bar":2}
assert root.bar.sm()=={"foo":1,"bar":2}
assert root.bar.sm({"foo":1,"foobar":3})._properties()=={"foo":1,"foobar":3}
assert root.bar.sm['foo']==1
assert root.bar.sm.contains("foobar")
root.bar.sm.erase("foobar")
assert not root.bar.sm.contains("foobar")
root.bar.sm.insert({"first":"bar","second":1})
assert root.bar.sm.keys()==["bar","foo"]
assert root.bar.rotation()==0
assert root.bar.rotation(20)==20
assert root.bar.rotation()==20


assert root.testString('hello')=='hello'

assert root.testDoubleIntOverload(1)==0
assert root.testDoubleIntOverload(1,1)==1
assert root.testDoubleIntOverload(1,1.1)==1
expectThrow(lambda: root.testDoubleIntOverload(1.1,1.1))
expectThrow(lambda: root.dummy({a:1}))
assert root.voidReturn()==None

# test type factories
from pyExample import *
foo=Foo(2)
fb=FooBar1()
assert foo.b()==2
assert fb.f.b()==0

# shared pointer support
assert root.sbar.f()==20
assert root.spoly.a()==5
