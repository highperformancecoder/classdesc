/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

class JNIProxy
{
    String name;
    JNIInterface intf;

    public JNIProxy(String name, JNIInterface intf)
    {
        this.name = name;
        this.intf = intf;
    }

    public Object call(String method, Object... args)
    {
        System.out.printf("in call %s, %d args\n",method,(args!=null)? args.length: 0);        
        return intf.call(name+"."+method, args);
    }
}
