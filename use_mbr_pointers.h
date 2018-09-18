/*
  @copyright Russell Standish 2018
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/
/**\file\brief
 defines overloads supporting -use_mbr_pointers, when old style object descriptors provided
*/
#ifndef USE_MBR_POINTERS_H
#define USE_MBR_POINTERS_H

#define CLASSDESC_USE_OLDSTYLE_MEMBER_OBJECTS(descriptor)       \
  namespace classdesc {                                         \
    template<class C, class T>                                  \
    typename enable_if<is_member_object_pointer<T>,void>::T     \
    descriptor(descriptor##_t& b, const string& d, C& o, T y)   \
    {descriptor(b,d,o.*y);}                                     \
                                                                \
    /* for static object members */                             \
    template<class C, class T>                                  \
    typename enable_if<is_object<T>,void>::T                    \
    descriptor(descriptor##_t& b, const string& d, C& o, T* y)  \
    {descriptor(b,d,*y);}                                       \
  }  

// do nothing for function objects - suitable for all serialisation descriptors
#define CLASSDESC_FUNCTION_NOP(descriptor)                      \
  namespace classdesc {                                         \
    /* bare function support */                                 \
    template<class T>                                           \
    typename enable_if<is_function<T>,void>::T                  \
    descriptor(descriptor##_t& b, const string& d, T* y)        \
    {}                                                          \
                                                                \
    /* -use_mbr_pointers support for member functions */                \
    template<class C, class T>                                          \
    typename enable_if<is_member_function_pointer<T>,void>::T           \
    descriptor(descriptor##_t&, const string&, C&, T) {}                \
                                                                        \
    /* -use_mbr_pointers support for function pointer members */        \
    template<class C, class T>                                          \
    typename enable_if<is_function<T>,void>::T                          \
    descriptor(descriptor##_t& b, const string& d, C, T*)               \
    {}                                                                  \
  }

#endif
