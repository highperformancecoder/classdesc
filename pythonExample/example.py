import example
r=example.static.root
assert r.bar.ch=='M'
r.bar.ch='A'
assert r.bar.ch=='A'
assert r.bar.bf==False
assert r.bar.bt==True
r.bar.bt=False
assert r.bar.bt==False
assert r.bar.c=='\r hello & 123 '
assert len(r.bar.c)==len('\r hello & 123 ')
assert len(r.bar.c1)==2
# doesn't work???
for i in r.bar.c1:
    assert i=='\r'
assert len(r.bar.d)==3
for i in range(len(r.bar.d)):
    assert r.bar.d[i]==i

assert len(r.bar.d1)==3
for i in range(len(r.bar.d1)):
    assert len(r.bar.d1[i])==2
    for j in range(len(r.bar.d1[i])):
        assert r.bar.d1[i][j]==2*i+j

r.bar.d1[0][1]=10
assert r.bar.d1[0][1]==10
        
assert len(r.bar.h)==3
for i in r.bar.h:
    assert i==2

assert len(r.bar.l)==3
for i in r.bar.h:
    assert i==2

r.bar.h[1]=3
assert r.bar.h[1]==3

r.bar.l[1]=3
assert r.bar.l[1]==3

assert r.bar.m[0]==5
assert r.bar.m[3]==2

try:
    r.bar.m[1]
    assert False
except:
    pass

r.bar.m[3]=1
assert r.bar.m[3]==1


assert len(r.bar.llex)==2
for i in r.bar.llex:
    assert len(i)==2
    for j in i:
        assert j=="hello"

assert "iex" not in r.bar.__dict__ # tests Exclude
assert len(r.bar.vs)==2
for i in r.bar.vs:
    assert i==' hello'

r.bar.vs[1]="world"
assert r.bar.vs[1]=="world"

assert r.bar.ef=="ea"
assert r.bar.getEF()==0
r.bar.ef="ec"
assert r.bar.getEF()==12

sm=r.bar.sm
assert len(sm)==0
sm['foo']=2
sm['bar']=3
assert len(sm)==2
assert sm['foo']==2
assert sm['bar']==3
for i in sm: assert i in sm

assert r.bar.vs0()==' hello'

from example import Foo
assert Foo.shello()=='hello'

# assign to a shared pointer target
r.bar1.fp.target=r.bar1.f
# chack we can examine a shared ppointer target
assert r.bar1.fp.target.a==r.bar1.f.a

b1=example.Bar1()
# assign a reference
b1.fp=r.bar1.fp
b1.fp.target.a=54
assert r.bar1.fp.target.a==54

nullRef=getattr(example,"classdesc::shared_ptr<Foo>")()
try:
    nullRef.target
    assert False
except RuntimeError:
    pass

# this doesn't work!!!
#b1.fp=nullRef
#try:
#    b1.fp.target
#    assert False
#except RuntimeError:
#    pass
