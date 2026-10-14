#ifndef FUNCTIONAL_HPP_
#define FUNCTIONAL_HPP_

#include "libj/type_traits.hpp"
#include "libj/xtoy.hpp"

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
} // namespace jstd
namespace jos {
struct parse_args;
} // namespace jos
namespace jstd {
template<unsigned... Is> struct seq{};
template<unsigned N, unsigned... Is>
struct gen_seq : gen_seq<N-1, N-1, Is...> {};
template<unsigned... Is>
struct gen_seq<0, Is...> : seq<Is...> {};

template<typename FuncPtr>
struct invoker;

template<typename ReturnType, typename... ArgTypes>
struct invoker<ReturnType(*)(ArgTypes...)>
{
private:
  template<unsigned... Is, typename ParseArgsType>
  static ReturnType run_impl(ReturnType(*fptr)(ArgTypes...), ParseArgsType& args, seq<Is...>)
  { return fptr(jstd::template cvt_arg<ArgTypes>(args.get_arg(Is))...); }
public:
  template<typename ParseArgsType>
  static ReturnType run(ReturnType(*fptr)(ArgTypes...), ParseArgsType& args)
  { return run_impl(fptr, args, gen_seq<sizeof... (ArgTypes)>{}); }
};

template<>
struct invoker<void(*)(jos::parse_args&)> {
  template<typename ParseArgsType>
  static void run(void(*f)(jos::parse_args&), ParseArgsType& args) 
  { f(args); }
};
} // namespace jstd
#endif // FUNCTIONAL_HPP_
