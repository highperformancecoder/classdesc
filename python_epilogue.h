#ifndef PYTHONOBJECT_EPILOGUE_H
#define PYTHONOBJECT_EPILOGUE_H

namespace classdesc
{
  template <class T, class Base>
  typename enable_if<ClassdescEnabledPythonType<T>,void>::T
  python(python_t& p, const string& d)
  {
    classdesc_access::access_python<Base>().template type<T>(p,d);
    if (is_same<T,Base>::value)
      p.getClass<T>().completed=true;
  }

  
  template <class T, int rank>
  void detail::ArrayGetRegisterClass<T,rank>::registerClass(python_t& p)
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
  template <> struct access_python<string>
  {
    template <class T>
    void type(classdesc::python_t&,const string&) {}
  };

  
  
}

#endif
