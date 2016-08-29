/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

import java.lang.reflect.*;

public class JNIProxyInvocation implements InvocationHandler
{
    public Object invoke(Object proxy, Method method, Object... args) throws Throwable
    {
        //need to look up return type of method and put in switch statement here
        System.out.printf("in %s, %d args\n",method.getName(),(args!=null)? args.length: 0);
        JNIInterface.call("ex."+method.getName(),args);
        return proxy;
    }
}
