struct Foo
{
  void a() {}
};

struct Bar: public Foo
{
  using Foo::a;
  void a(int) {}
};
