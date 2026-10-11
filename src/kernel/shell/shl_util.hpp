#ifndef SHL_UTIL_HPP_
#define SHL_UTIL_HPP_

#include "libj/exception.hpp"

namespace jos {
struct parse_args {
public:
  int args_cnt;
  static constexpr unsigned MAX_ARGS = 2;
  static constexpr unsigned MAX_ARG_SIZE = 128;
  char args[MAX_ARGS][MAX_ARG_SIZE] = {0};

  explicit parse_args(const char buffer[]) 
    : args_cnt{0} 
  {
    int  i{0};
    int ai{0};
    int ci{0};
    while(buffer[i] != '\0' && ai < MAX_ARGS) {
      if(buffer[i] == ' ') {
        args[ai][ci] = '\0';
        ai++;
        ci=0;
      } else if(ci < (MAX_ARG_SIZE-1)) {
        args[ai][ci] = buffer[i];
        ci++;
      }
      i++;
    }
    if (ci > 0 && ai < MAX_ARGS) {
      args[ai][ci] = '\0';
      ai++;
    }
    args_cnt = ai;
  }
};

template<unsigned int Index, unsigned int Total>
struct auto_switch;

template<unsigned int Index, unsigned int Total>
struct auto_switch {
  template<typename S>
  static auto& get(unsigned i, S& lambdas) {
    using ftype = decltype(lambdas.placeholder);
    if(i==Index) { return reinterpret_cast<ftype*>(&lambdas)[Index]; }
    return auto_switch<Index+1, Total>::get(i, lambdas); 
  }
};

template<unsigned int Total>
struct auto_switch<Total-1, Total> {
  template<typename S>
  static auto& get(unsigned i, S& lambdas) {
    using ftype = decltype(lambdas.placeholder);
    if(i==Total-1) { return reinterpret_cast<ftype*>(&lambdas)[Total-1]; }
    index_out_of_bounds("index given is greater than or equal to total");
    // to make compiler happy
    return *reinterpret_cast<ftype*>(0);
  }
};
} // namespace jos
#endif // SHL_UTIL_HPP_
