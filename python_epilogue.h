#ifndef PYTHONOBJECT_EPILOGUE_H
#define PYTHONOBJECT_EPILOGUE_H

namespace classdesc
{
  template <class T>
  typename enable_if<ClassdescEnabledPythonType<T>,void>::T
  python(python_t& p, const string& d, T& a) {
    classdesc_access::access_python<T>()(p,d,a);
    //pythonObject_t::getClass<T>().completed=true;
  }

  template <class T, class Base>
  typename enable_if<ClassdescEnabledPythonType<T>,void>::T
  python(python_t& p, const string& d)
  {
    classdesc_access::access_python<Base>().template type<T>(p,d);
    if (is_same<T,Base>::value)
      p.getClass<T>().completed=true;
  }

  
  template <class T, int R>
  struct tn<detail::ArrayGet<T,R> >
  {
    static std::string name()
    {return "detail::ArrayGet<"+typeName<T>()+","+std::to_string(R)+">";}
  };
  
  template <class T, int R>
  void detail::ArrayGet<T,R>::registerClass(python_t& p)
  {
    python_t::Class<ArrayGet<T,R> >& c=p.getClass<ArrayGet<T,R> >();
    if (!c.completed)
      {
        c.def("__len__",&ArrayGet<T,R>::len).
          def("__getitem__",&ArrayGet<T,R>::getItem);
        c.completed=true;
      }
  }

  template <class T>
  void detail::ArrayGet<T,1>::registerClass(python_t& p)
  {
    python_t::Class<ArrayGet<T,1> >& c=p.getClass<ArrayGet<T,1> >();
    if (!c.completed)
      {
        c.def("__len__",&ArrayGet<T,1>::len).
          def("__getitem__",&ArrayGet<T,1>::getItem).
          def("__setitem__",&ArrayGet<T,1>::setItem);
        c.completed=true;
      }
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
