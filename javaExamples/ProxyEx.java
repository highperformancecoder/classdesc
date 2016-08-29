/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

import java.lang.reflect.*;

public class ProxyEx
{
    public static void main(String[] args)
    {
        ProxyExInterface foo=   (ProxyExInterface)
            Proxy.newProxyInstance(
                                   ClassLoader.getSystemClassLoader(),
                                   new Class<?>[]{ProxyExInterface.class},
                                    new ProxyInvocation());
        foo.setfoo(1);
    }
}
