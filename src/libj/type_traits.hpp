#ifndef STL_HPP_
#define STL_HPP_

namespace jstd {
template<typename T>
struct remove_reference { using type = T; };
template<typename T>
struct remove_reference<T&> { using type = T; };
template<typename T>
struct remove_reference<T&&> { using type = T; };

template<typename T>
struct is_lvalue_reference { static constexpr bool value = false; };
template<typename T>
struct is_lvalue_reference<T&> { static constexpr bool value = true; };
} // namespace jstd
#endif // STL_HPP_
