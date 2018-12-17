#ifndef PYTHONOBJECT_EPILOGUE_H
#define PYTHONOBJECT_EPILOGUE_H

namespace classdesc
{
  template <class T, class Base>
  typename enable_if<ClassdescEnabledPythonType<Base>,void>::T
  python(python_t& p, const string& d)
  {
    classdesc_access::access_python<Base>().template type<T>(p,d);
    if (is_same<T,Base>::value)
      p.getClass<T>().completed=true;
  }

  
  template <class T, int rank>
  void pythonDetail::ArrayGetRegisterClass<T,rank>::registerClass(python_t& p)
  {
    auto& c=p.getClass<ArrayGet<T,rank> >();
    if (!c.completed)
      {
        c.def("__len__",&arrayMemLen<ArrayGet<T,rank>, T>).
          def("__getitem__",&ArrayGet<T,rank>::get);
        if (rank==1)
          c.def("__setitem__",&ArrayGet<T,rank>::set);
      }
    ArrayGetRegisterClass<typename std::remove_extent<T>::type,rank-1>::registerClass(p);
  }
}

namespace classdesc_access
{
  // define anything not yet defined to be the null descriptor
  template <class T> struct access_python<T,void>: public classdesc::NullDescriptor<classdesc::python_t> {};
}

#endif
