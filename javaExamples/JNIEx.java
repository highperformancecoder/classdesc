/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

import java.lang.reflect.*;

public class JNIEx
{

    public static Object call(Object obj, String method, Object... args) throws Throwable
    {
        System.out.printf("in call %s, %d args\n",method,(args!=null)? args.length: 0);        
        System.out.println(obj.getClass().getName());
        System.out.println(obj.getClass().getMethod(method,Object[].class).getName());
        if (args==null) args=new Object[0];
        return obj.getClass().getMethod(method,Object[].class).invoke(args);
    }

    public static void main(String[] arg)
    {
        JNIInterface loader=new JNIInterface("JNIEx");
        Class<?> JNIEx;
        try
            {
                JNIEx = loader.loadClass("ExampleIntf");
            }
        catch (ClassNotFoundException e1)
            {
                System.out.println("Class not found exception thrown");
                return;
            }
        Method[] methods = JNIEx.getDeclaredMethods();
        for (Method m: methods)
            {
                System.out.printf("%s %s\n",m.getName(), m.getReturnType().getName());
            }

        System.out.println("-----------------");
        {
            Class<?> Ex = Proxy.getProxyClass(loader, new Class<?>[]{JNIEx});
            Object ex = Proxy.newProxyInstance(loader, new Class<?>[]{JNIEx}, 
                                            new JNIProxyInvocation());
            for (Method m: Ex.getDeclaredMethods())
                {
                    try
                        {
                            ex.getClass().getMethod(m.getName(),m.getParameterTypes());
                        }
                    catch (Throwable e2)
                        {
                            System.out.printf("Error in getting method %s: %s\n",
                                              m.getName(), e2.getMessage());
                        } 
                }

          try
              {
                  JNIInterface.call("ex.printFoo",0);
                  JNIInterface.call("ex.foo",2);
                  JNIInterface.call("ex.printFoo",0);
                  JNIInterface.call("ex.setFoo",3);
                  JNIInterface.call("ex.printFoo",0);
                  System.out.println(JNIInterface.call("ex.getFoo",0));
                  JNIInterface.call("ex.printBar",0);
                  JNIInterface.call("ex.setBar",2.0);
                  JNIInterface.call("ex.printBar",0);
                  System.out.println(JNIInterface.call("ex.getBar",2));
                }
            catch (Throwable e1)
                {
                    System.out.println("Error:"+e1.getMessage());
                }
        }

//        //Now try direct call
//        {
//            Example ex = new Example();
//            try
//                {
//                    System.out.println("b4 ex. calls");
//                    ex.printFoo();
//                    ex.printBar();
//                }
//            catch (Throwable e1)
//                {
//                    System.out.println(e1.getMessage());
//                }
//        }      
    }
}

