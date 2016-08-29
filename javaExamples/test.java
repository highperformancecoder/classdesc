/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/


class test
{
    public static void main(String[] arg)
    {
        System.loadLibrary("JNIEx2");
        Example foo=new Example();
        assert foo.foo(1)==1;
        foo.printFoo();
        foo.setFoo(2);
        assert foo.getFoo()==2;
        foo.printFoo();
        System.out.printf("%d\n",foo.getFoo());
        System.out.printf("%g\n",foo.bar(3));
        assert foo.bar(3)==3;
        foo.printBar();
        foo.setBar(4);
        assert foo.getBar()==4;
        foo.printBar();
    }
}

