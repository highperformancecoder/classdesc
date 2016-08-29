/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

public abstract class CppObject
{
    String register;
    protected abstract void auto();
    public CppObject() {auto();}
}

