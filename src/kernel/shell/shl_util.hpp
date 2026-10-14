#ifndef SHL_UTIL_HPP_
#define SHL_UTIL_HPP_

#include "libj/exception.hpp"
#include "libj/memory.hpp"
#include "macros.hpp"

namespace jos {
struct parse_args {
  unsigned args_cnt;
  static constexpr unsigned MAX_ARGS = 4;
  static constexpr unsigned MAX_ARG_SIZE = 128;
  char args[MAX_ARGS][MAX_ARG_SIZE] = {0};

  explicit parse_args(const char buffer[]) 
    : args_cnt{0} 
  {
    unsigned i{0}, ai{0}, ci{0};
    bool in_quotes{false};
    while (buffer[i] != '\0' && ai < MAX_ARGS) {
      char c = buffer[i];
      i++;
      if(c == '"') { in_quotes = !in_quotes; continue; }
      if(c == ' ' && !in_quotes) {
        if(ci > 0) { 
          args[ai][ci] = '\0';
          ai++;
          ci = 0;
          continue;
        }
      }
      if(ci < MAX_ARG_SIZE-1) {
        args[ai][ci] = c;
        ci++;
      } else {
        args[ai][MAX_ARG_SIZE-1] = '\0';
      }
    }
    if (ci > 0 && ai < MAX_ARGS) {
      args[ai][ci] = '\0';
      ai++;
    }
    args_cnt = ai;
  }
  [[nodiscard]] const char* get_arg(int i) const {
    if (i >= 0 && i < args_cnt) { return args[i]; }
    return "";
  }
};
} // namespace jos
#endif // SHL_UTIL_HPP_
