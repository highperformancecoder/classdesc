from example import root as r
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
        
assert len(r.bar.h)==3
for i in r.bar.h:
    assert i==2

# TODO
#r.bar.h[1]=3
#assert r.bar.h[1]==3

assert len(r.bar.l)==3
for i in r.bar.h:
    assert i==2

# TODO
#r.bar.l[1]=3
#assert r.bar.l[1]==3

assert len(r.bar.llex)==2
for i in r.bar.llex:
    assert len(i)==2
    for j in i:
        assert j=="hello"

assert "iex" not in r.bar.__dict__ # tests Exclude
assert len(r.bar.vs)==2
for i in r.bar.vs:
    assert i==' hello'

# TODO
#r.bar.vs[1]="world"
#assert r.bar.vs[1]=="world"

    
# TODO
# r.bar.sef

assert r.bar.vs0()==' hello'

from example import Foo
assert Foo.shello()=='hello'
