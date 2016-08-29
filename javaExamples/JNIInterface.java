/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

class JNIInterface extends ClassLoader
{
    public JNIInterface(String dynamicLib)
    {
        System.loadLibrary(dynamicLib);
    }
    protected Class<?> findClass(String name) throws ClassNotFoundException
    {
        byte[] classData=getExampleInterface();
        return defineClass(name,classData,0,classData.length);
    }
    // return binary rep of C++ class interface
    static public native byte[] getExampleInterface();
    static public native Object call(String name, Object... args);
}


    
        
