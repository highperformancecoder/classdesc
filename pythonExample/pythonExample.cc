#include "pythonExample.h"
#include "classdesc_epilogue.h"

Root root;
const int Foo::csi;
int Foo::si=25;

void registerRoot()
{
  addPythonObject("root1",root);
  addPythonObject("root1",root);
  addPythonObject("example.root1",root);
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(Bar_overloadExample0,overloadExample,0,0);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(Bar_overloadExample1,overloadExample,1,2);

//struct VectorTupleConverter
//{
//  static std::vector<int> execute(const boost::python::object& x) {
//    std::vector<int> r;
//    for (size_t i=0; i<len(x); ++i)
//      r.push_back(extract<int>(x[i]));
//    return r;
//  }
//};

//namespace boost
//{
//  namespace python
//  {
//    template <class T>
//    struct extract<std::vector<T>>
//    {
//      typedef std::vector<T> result_type;
//      PyObject* obj;
//      extract(PyObject* o): obj(o) {}
//      extract(const object& o): obj(o.ptr()) {}
//      result_type operator()() const {
//        auto eo=extract<object>(obj)();
//        std::vector<T> r;
//        for (size_t i=0; i<len(eo); ++i)
//          r.push_back(eo[i]);
//        return r;
//      }
//      operator result_type() const {
//        return (*this)();
//      }
//      bool check() const {
//        try {
//          (*this)();
//          return true;
//        }
//        catch (...) {return false;}
//      }
//    };
//  }
//}




BOOST_PYTHON_MODULE(example)
{
  python_t p;
  p.defineClass<Root>();
  p.addFunctional("registerRoot1",registerRoot);
  p.addObject("static.root",root);
  p.addObject("root",root);
  addPythonObject("root",root);
  Bar& (Bar::*o1)()=&Bar::overloadExample;
  int (Bar::*o2)(int,int) const=&Bar::overloadExample;
  p.getClass<Bar>().overload("overloadExample",o1);
  //  p.getClass<Bar>().overload("overloadExample",o2,Bar_overloadExample1(args("x","y")));
  p.getClass<Bar>().overload("overloadExample",o2,Bar_overloadExample1());

//  p.defineClass<Vector>();
//  p.getClass<Vector>().def(boost::python::init<const boost::python::list&>());
//  boost::python::implicitly_convertible<boost::python::list,std::vector<int>>();

  //  boost::python::lvalue_from_pytype<VectorTupleConverter, &PyList_Type>();
//  boost::python::lvalue_from_pytype<VectorTupleConverter, &PyTuple_Type>();
}

