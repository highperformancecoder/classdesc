#ifndef PYTHONOBJECT_EPILOGUE_H
#define PYTHONOBJECT_EPILOGUE_H

namespace classdesc
{
  template <class T>
  typename enable_if<ClassdescEnabledPythonType<T>,void>::T
  pythonObject(pythonObject_t& p, const string& d, T& a) {
    classdesc_access::access_pythonObject<T>()(p,d,a);
    //pythonObject_t::getClass<T>().completed=true;
  }

  
}

#endif
