#ifndef FUNCTIONAL_HPP_
#define FUNCTIONAL_HPP_

#include "type_traits.hpp"

namespace jstd {
template<typename T>
inline constexpr T&& 
forward(typename remove_reference<T>::type& t) noexcept
{ return static_cast<T&&>(t); }

template<typename T>
inline constexpr T&&
forward(typename remove_reference<T>::type&& t) noexcept 
{
  static_assert(!is_lvalue_reference<T>::value, "template arg for T is lvalue reference type");
  return static_cast<T&&>(t);
}

struct parse_args;

template<typename Func>
struct invoker;

template<>
struct invoker<void(*)()> {
  template<typename... Args>
  static void run(void(*f)(), Args&&...) {
    f();
  }
};

template<>
struct invoker<void(*)(parse_args&)> {
  template<typename... Args>
  static void run(void(*f)(parse_args&), Args&&... args) {
    f(forward<Args>(args)...);
  }
};
} // namespace jstd
#endif // FUNCTIONAL_HPP_
