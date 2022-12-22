#ifndef CLASSDESC_FUNCTIONDB20
#define CLASSDESC_FUNCTIONDB20
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 1> 
{
  typedef A1 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 1> 
{
  typedef A1 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 1> 
{
  typedef A1 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 1> 
{
  typedef A1 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 1> 
{
  typedef A1 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 1> 
{
  typedef A1 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 2> 
{
  typedef A2 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 2> 
{
  typedef A2 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 2> 
{
  typedef A2 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 2> 
{
  typedef A2 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 2> 
{
  typedef A2 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 2> 
{
  typedef A2 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 3> 
{
  typedef A3 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 3> 
{
  typedef A3 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 3> 
{
  typedef A3 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 3> 
{
  typedef A3 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 3> 
{
  typedef A3 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 3> 
{
  typedef A3 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 4> 
{
  typedef A4 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 4> 
{
  typedef A4 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 4> 
{
  typedef A4 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 4> 
{
  typedef A4 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 4> 
{
  typedef A4 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 4> 
{
  typedef A4 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 5> 
{
  typedef A5 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 5> 
{
  typedef A5 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 5> 
{
  typedef A5 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 5> 
{
  typedef A5 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 5> 
{
  typedef A5 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 5> 
{
  typedef A5 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 6> 
{
  typedef A6 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 6> 
{
  typedef A6 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 6> 
{
  typedef A6 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 6> 
{
  typedef A6 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 6> 
{
  typedef A6 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 6> 
{
  typedef A6 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 7> 
{
  typedef A7 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 7> 
{
  typedef A7 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 7> 
{
  typedef A7 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 7> 
{
  typedef A7 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 7> 
{
  typedef A7 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 7> 
{
  typedef A7 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 8> 
{
  typedef A8 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 8> 
{
  typedef A8 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 8> 
{
  typedef A8 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 8> 
{
  typedef A8 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 8> 
{
  typedef A8 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 8> 
{
  typedef A8 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 9> 
{
  typedef A9 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 9> 
{
  typedef A9 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 9> 
{
  typedef A9 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 9> 
{
  typedef A9 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 9> 
{
  typedef A9 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 9> 
{
  typedef A9 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 10> 
{
  typedef A10 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 10> 
{
  typedef A10 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 10> 
{
  typedef A10 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 10> 
{
  typedef A10 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 10> 
{
  typedef A10 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 10> 
{
  typedef A10 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 11> 
{
  typedef A11 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 11> 
{
  typedef A11 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 11> 
{
  typedef A11 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 11> 
{
  typedef A11 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 11> 
{
  typedef A11 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 11> 
{
  typedef A11 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 12> 
{
  typedef A12 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 12> 
{
  typedef A12 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 12> 
{
  typedef A12 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 12> 
{
  typedef A12 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 12> 
{
  typedef A12 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 12> 
{
  typedef A12 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 13> 
{
  typedef A13 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 13> 
{
  typedef A13 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 13> 
{
  typedef A13 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 13> 
{
  typedef A13 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 13> 
{
  typedef A13 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 13> 
{
  typedef A13 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 14> 
{
  typedef A14 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 14> 
{
  typedef A14 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 14> 
{
  typedef A14 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 14> 
{
  typedef A14 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 14> 
{
  typedef A14 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 14> 
{
  typedef A14 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 15> 
{
  typedef A15 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 15> 
{
  typedef A15 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 15> 
{
  typedef A15 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 15> 
{
  typedef A15 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 15> 
{
  typedef A15 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 15> 
{
  typedef A15 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 16> 
{
  typedef A16 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 16> 
{
  typedef A16 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 16> 
{
  typedef A16 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 16> 
{
  typedef A16 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 16> 
{
  typedef A16 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 16> 
{
  typedef A16 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 17> 
{
  typedef A17 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 17> 
{
  typedef A17 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 17> 
{
  typedef A17 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 17> 
{
  typedef A17 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 17> 
{
  typedef A17 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 17> 
{
  typedef A17 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 18> 
{
  typedef A18 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 18> 
{
  typedef A18 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 18> 
{
  typedef A18 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 18> 
{
  typedef A18 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 18> 
{
  typedef A18 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 18> 
{
  typedef A18 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 19> 
{
  typedef A19 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 19> 
{
  typedef A19 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 19> 
{
  typedef A19 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 19> 
{
  typedef A19 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 19> 
{
  typedef A19 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 19> 
{
  typedef A19 T;
  typedef T type;
};
#endif
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 20> 
{
  typedef A20 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20), 20> 
{
  typedef A20 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const, 20> 
{
  typedef A20 T;
  typedef T type;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods
template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 20> 
{
  typedef A20 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept, 20> 
{
  typedef A20 T;
  typedef T type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arg<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept, 20> 
{
  typedef A20 T;
  typedef T type;
};
#endif
template <class F, template<class> class P>
struct AllArgs<F,P,20>
{
   static const bool value=true  && P<typename Arg<F,1>::T>::value  && P<typename Arg<F,2>::T>::value  && P<typename Arg<F,3>::T>::value  && P<typename Arg<F,4>::T>::value  && P<typename Arg<F,5>::T>::value  && P<typename Arg<F,6>::T>::value  && P<typename Arg<F,7>::T>::value  && P<typename Arg<F,8>::T>::value  && P<typename Arg<F,9>::T>::value  && P<typename Arg<F,10>::T>::value  && P<typename Arg<F,11>::T>::value  && P<typename Arg<F,12>::T>::value  && P<typename Arg<F,13>::T>::value  && P<typename Arg<F,14>::T>::value  && P<typename Arg<F,15>::T>::value  && P<typename Arg<F,16>::T>::value  && P<typename Arg<F,17>::T>::value  && P<typename Arg<F,18>::T>::value  && P<typename Arg<F,19>::T>::value   && P<typename Arg<F,20>::T>::value;
};


template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arity<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)> 
{
    static const int V=20;
    static const int value=20;
};

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Return<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)> 
{
    typedef R T;
    typedef R type;
};

template <class C,class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arity<R (* C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)> 
{
    static const int V=20;
    static const int value=20;
};

template <class C,class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Return<R (* C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arity<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)> 
{
    static const int V=20;
    static const int value=20;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Return<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arity<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const> 
{
    static const int V=20;
    static const int value=20;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Return<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct ClassOf<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct ClassOf<R (*C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct ClassOf<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct is_member_function_ptr<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)>
{
   static const bool value=true;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct is_member_function_ptr<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const>
{
   static const bool value=true;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct is_const_method<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const>
{
   static const bool value=true;
};

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct is_nonmember_function_ptr<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)>
{
   static const bool value=true;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct is_nonmember_function_ptr<R (*C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)>
{
   static const bool value=true;
};

template <class C, class D, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20>
class bound_method<C, R (D::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)>
{
    typedef R (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20);
    C* obj;
    M method;
    public:
    static const int arity=20;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> >, R>::T
    operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9,A10 a10,A11 a11,A12 a12,A13 a13,A14 a14,A15 a15,A16 a16,A17 a17,A18 a18,A19 a19,A20 a20) const {return (obj->*method)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20>
class bound_method<C, void (D::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)>
{
    typedef void (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20);
    C* obj;
    M method;
    public:
    static const int arity=20;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9,A10 a10,A11 a11,A12 a12,A13 a13,A14 a14,A15 a15,A16 a16,A17 a17,A18 a18,A19 a19,A20 a20) const {(obj->*method)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20>
class bound_method<const C, R (D::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)>
{
    typedef R (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20);
    const C* obj;
    M method;
    public:
    static const int arity=20;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    R operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9,A10 a10,A11 a11,A12 a12,A13 a13,A14 a14,A15 a15,A16 a16,A17 a17,A18 a18,A19 a19,A20 a20) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20>
class bound_method<const C, void (D::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20)>
{
    typedef void (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20);
    const C* obj;
    M method;
    public:
    static const int arity=20;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9,A10 a10,A11 a11,A12 a12,A13 a13,A14 a14,A15 a15,A16 a16,A17 a17,A18 a18,A19 a19,A20 a20) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20>
class bound_method<C, R (D::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const>
{
    typedef R (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const;
    C& obj;
    M method;
    public:
    static const int arity=20;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    R operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9,A10 a10,A11 a11,A12 a12,A13 a13,A14 a14,A15 a15,A16 a16,A17 a17,A18 a18,A19 a19,A20 a20) const {return (obj.*method)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20);}
    static const bool is_const=true;
};

template <class C, class D, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20>
class bound_method<C, void (D::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const>
{
    typedef void (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const;
    C& obj;
    M method;
    public:
    static const int arity=20;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    void operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9,A10 a10,A11 a11,A12 a12,A13 a13,A14 a14,A15 a15,A16 a16,A17 a17,A18 a18,A19 a19,A20 a20) const {(obj.*method)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20);}
    static const bool is_const=true;
};

#if defined(__cplusplus) && __cplusplus>=201703L
// noexcept methods

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arity<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept> 
{
    static const int V=20;
    static const int value=20;
};

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Return<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C,class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arity<R (* C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept> 
{
    static const int V=20;
    static const int value=20;
};

template <class C,class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Return<R (* C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arity<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept> 
{
    static const int V=20;
    static const int value=20;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Return<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Arity<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept> 
{
    static const int V=20;
    static const int value=20;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct Return<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept> 
{
    typedef R T;
    typedef R type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct ClassOf<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct ClassOf<R (*C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct ClassOf<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept> 
{
    typedef C T;
    typedef C type;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct is_member_function_ptr<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept>
{
   static const bool value=true;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct is_member_function_ptr<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept>
{
   static const bool value=true;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct is_const_method<R (C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept>
{
   static const bool value=true;
};

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct is_nonmember_function_ptr<R (*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept>
{
   static const bool value=true;
};

template <class C, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20> 
struct is_nonmember_function_ptr<R (*C::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept>
{
   static const bool value=true;
};

template <class C, class D, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20>
class bound_method<C, R (D::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept>
{
    typedef R (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20);
    C* obj;
    M method;
    public:
    static const int arity=20;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> >, R>::T
    operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9,A10 a10,A11 a11,A12 a12,A13 a13,A14 a14,A15 a15,A16 a16,A17 a17,A18 a18,A19 a19,A20 a20) const {return (obj->*method)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20>
class bound_method<C, void (D::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept>
{
    typedef void (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20);
    C* obj;
    M method;
    public:
    static const int arity=20;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9,A10 a10,A11 a11,A12 a12,A13 a13,A14 a14,A15 a15,A16 a16,A17 a17,A18 a18,A19 a19,A20 a20) const {(obj->*method)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20);}
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20>
class bound_method<const C, R (D::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept>
{
    typedef R (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20);
    const C* obj;
    M method;
    public:
    static const int arity=20;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    R operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9,A10 a10,A11 a11,A12 a12,A13 a13,A14 a14,A15 a15,A16 a16,A17 a17,A18 a18,A19 a19,A20 a20) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20>
class bound_method<const C, void (D::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) noexcept>
{
    typedef void (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20);
    const C* obj;
    M method;
    public:
    static const int arity=20;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(const C& obj, M method): obj(&obj), method(method) {}
    typename enable_if<Not<classdesc::is_const<C> > >::T
    operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9,A10 a10,A11 a11,A12 a12,A13 a13,A14 a14,A15 a15,A16 a16,A17 a17,A18 a18,A19 a19,A20 a20) const {
        throw std::runtime_error("cannot call method, inappropriate argument type");
    }
    void rebind(C& newObj) {obj=&newObj;}
    static const bool is_const=false;
};

template <class C, class D, class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20>
class bound_method<C, R (D::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept>
{
    typedef R (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const;
    C& obj;
    M method;
    public:
    static const int arity=20;
    typedef R Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    R operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9,A10 a10,A11 a11,A12 a12,A13 a13,A14 a14,A15 a15,A16 a16,A17 a17,A18 a18,A19 a19,A20 a20) const {return (obj.*method)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20);}
    static const bool is_const=true;
};

template <class C, class D, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10, class A11, class A12, class A13, class A14, class A15, class A16, class A17, class A18, class A19, class A20>
class bound_method<C, void (D::*)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const noexcept>
{
    typedef void (D::*M)(A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19,A20) const;
    C& obj;
    M method;
    public:
    static const int arity=20;
    typedef void Ret;
    template <int i> struct Arg: public functional::Arg<M,i> {};
    bound_method(C& obj, M method): obj(obj), method(method) {}
    void operator()(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9,A10 a10,A11 a11,A12 a12,A13 a13,A14 a14,A15 a15,A16 a16,A17 a17,A18 a18,A19 a19,A20 a20) const {(obj.*method)(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20);}
    static const bool is_const=true;
};
#endif

template <class F, class Args> 
typename enable_if<And<AllArgs<F, is_rvalue>, Eq<Arity<F>::value, 20> >, 
   typename Return<F>::T>::T
apply_nonvoid_fn(F f, Args& a, Fdummy<F> dum=0)
{
  return f(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],a[10],a[11],a[12],a[13],a[14],a[15],a[16],a[17],a[18],a[19]);
}

/*
 TODO: if any of the arguments to f are lvalues, we need to construct temporaries,
 which require C++-11 ability to test for the existence of a copy constructor. 
 If the return type is not copy constructable, the user must arrange for the return value to be discarded
*/

template <class F, class Args> 
typename enable_if<And<AllArgs<F, is_rvalue>, Eq<Arity<F>::value, 20> >,
    void>::T
apply_void_fn(F f, Args& a, Fdummy<F> dum=0)
{
  f(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],a[10],a[11],a[12],a[13],a[14],a[15],a[16],a[17],a[18],a[19]);
}

template <class Buffer, class F>
typename enable_if<Eq<Arity<F>::value, 20>, typename Return<F>::T>::T
callOnBuffer(Buffer& buffer, F f)
{
  typename remove_const<typename remove_reference<typename Arg<F,1>::T>::type>::type a1;
  buffer>>a1;
  typename remove_const<typename remove_reference<typename Arg<F,2>::T>::type>::type a2;
  buffer>>a2;
  typename remove_const<typename remove_reference<typename Arg<F,3>::T>::type>::type a3;
  buffer>>a3;
  typename remove_const<typename remove_reference<typename Arg<F,4>::T>::type>::type a4;
  buffer>>a4;
  typename remove_const<typename remove_reference<typename Arg<F,5>::T>::type>::type a5;
  buffer>>a5;
  typename remove_const<typename remove_reference<typename Arg<F,6>::T>::type>::type a6;
  buffer>>a6;
  typename remove_const<typename remove_reference<typename Arg<F,7>::T>::type>::type a7;
  buffer>>a7;
  typename remove_const<typename remove_reference<typename Arg<F,8>::T>::type>::type a8;
  buffer>>a8;
  typename remove_const<typename remove_reference<typename Arg<F,9>::T>::type>::type a9;
  buffer>>a9;
  typename remove_const<typename remove_reference<typename Arg<F,10>::T>::type>::type a10;
  buffer>>a10;
  typename remove_const<typename remove_reference<typename Arg<F,11>::T>::type>::type a11;
  buffer>>a11;
  typename remove_const<typename remove_reference<typename Arg<F,12>::T>::type>::type a12;
  buffer>>a12;
  typename remove_const<typename remove_reference<typename Arg<F,13>::T>::type>::type a13;
  buffer>>a13;
  typename remove_const<typename remove_reference<typename Arg<F,14>::T>::type>::type a14;
  buffer>>a14;
  typename remove_const<typename remove_reference<typename Arg<F,15>::T>::type>::type a15;
  buffer>>a15;
  typename remove_const<typename remove_reference<typename Arg<F,16>::T>::type>::type a16;
  buffer>>a16;
  typename remove_const<typename remove_reference<typename Arg<F,17>::T>::type>::type a17;
  buffer>>a17;
  typename remove_const<typename remove_reference<typename Arg<F,18>::T>::type>::type a18;
  buffer>>a18;
  typename remove_const<typename remove_reference<typename Arg<F,19>::T>::type>::type a19;
  buffer>>a19;
  typename remove_const<typename remove_reference<typename Arg<F,20>::T>::type>::type a20;
  buffer>>a20;
  return f(
a1
,
a2
,
a3
,
a4
,
a5
,
a6
,
a7
,
a8
,
a9
,
a10
,
a11
,
a12
,
a13
,
a14
,
a15
,
a16
,
a17
,
a18
,
a19
,
a20
  );
}
#endif
