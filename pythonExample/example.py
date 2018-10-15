import example
r=example.static.root
assert r.bar.ch=='M'
r.bar.ch='A'
assert r.bar.ch=='A'
print(r.bar.af)
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

# TODO - get static_properties to work properly
assert r.bar.ef.fget()=="ea"
assert r.bar.getEF()==0
r.bar.ef.fset("ec")
assert r.bar.getEF()==12
# TODO
# r.bar.sef

assert r.bar.vs0()==' hello'

from example import Foo
assert Foo.shello()=='hello'
