#ifndef PAIR_HPP_
#define PAIR_HPP_

#include "libj/functional.hpp"

namespace jstd {
template<typename T0, typename T1>
struct pair {
  T0 val1;
  T1 val2;

  template<typename U0, typename U1>
  pair(U0 other1, U1 other2) 
    : val1(forward<U0>(other1)), val2(forward<U1>(other2)) 
  {
  }
  template<unsigned I>
  auto& get() {
    if constexpr(I==0) { return val1; }
    if constexpr(I==1) { return val2; }
  }
  template<unsigned I>
  constexpr const auto& get() const {
    if constexpr(I==0) { return val1; }
    if constexpr(I==1) { return val2; }
  }
};
} // namespace jstd
#endif // PAIR_HPP_
