#ifndef SHELL_FUNCS_HPP_
#define SHELL_FUNCS_HPP_
#include "macros.hpp"
//#include "vga.hpp"
#include "libj/str_fns.hpp"
//#include "libj/functional.hpp"
#include "libj/exception.hpp"
#include "libj/pair.hpp"
#include "quit.hpp"
#include "shell/shl_util.hpp"
//#include "ide.hpp"
//#include "libj/memory.hpp"
#include "shell/shl_cmd_impls.hpp"

namespace jos {
struct shell_funcs {
private:
  using command = jstd::pair<const char*, void(*)(shell_funcs&, parse_args&)>;
  using command_fn = void(*)(shell_funcs&, parse_args&);
  template<auto Fn, bool uses_shell, bool uses_args>
  static constexpr command_fn cmd() {
    return [](shell_funcs& sf, parse_args& args) {
      if constexpr (uses_shell && uses_args) {
        Fn(sf, args);
      } else if constexpr (uses_shell) {
        Fn(sf);
      } else if constexpr (uses_args) {
        Fn(args);
      } else {
        Fn();
      }
    };
  }

  static void noop() {}

public:
  static constexpr unsigned FNS_SIZE = 7;
private:
  command funcs[FNS_SIZE] = {
    {"", cmd<noop, false, false>()},
    {"clear", cmd<clr, false, false>()},
    {"quit", cmd<quit, false, false>()},
    {"q", cmd<quit, false, false>()},
    {"readsec", cmd<readsec_cmd_impl, false, true>()},
    {"writesec", cmd<writesec_cmd_impl, false, true>()},
    {"help", cmd<help_cmd_impl, true, false>()},
  };

public:

  const char* get_fn_name(unsigned i) {
    return funcs[i].get<0>();
//    return auto_switch<0, FNS_SIZE>::get(i, funcs).template get<0>();
  }
  command get(unsigned i) {
    //return auto_switch<0, FNS_SIZE>::get(i, funcs);
    if(i >= FNS_SIZE) { 
      jstd::index_out_of_bounds("tried to access function that doesnt exist");
    }
    return funcs[i];
  }
  void call(unsigned i, parse_args& args) {
    if (i >= FNS_SIZE) return;
    auto fptr = funcs[i].template get<1>();
    fptr(*this, args);
  }
};

inline void help_cmd_impl(shell_funcs& sf) {
  for (unsigned j{1}; j < shell_funcs::FNS_SIZE; ++j) {
    if(jstd::strcmp(sf.get_fn_name(j), "help") != 0) {
      ld_str(sf.get_fn_name(j));
      ld_ch('\n');
    }
  }
}
} // namespace jos
#endif // SHELL_FUNCS_HPP_
