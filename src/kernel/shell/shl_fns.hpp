#ifndef SHELL_FUNCS_HPP_
#define SHELL_FUNCS_HPP_
#include "macros.hpp"
#include "vga.hpp"
#include "libj/str_fns.hpp"
#include "libj/functional.hpp"
#include "libj/exception.hpp"
#include "libj/pair.hpp"
#include "quit.hpp"
#include "shell/shl_util.hpp"

namespace jos {
void prompt() {
  ld_str(PROMPT);
}

struct shell_funcs {
private:
  struct private_fns_struct {
    template<typename T>
    using spair = pair<const char*, T>;
    template<typename ReturnType, typename... ArgTypes>
    using fpair = pair<const char*, ReturnType(*)(ArgTypes...)>;

    fpair<void> placeholder{"", [](){}};
    fpair<void> clear{"clear", [](){ clr(); }};
    fpair<void> quit1{"quit", [](){ quit(); }};
    fpair<void> quit2{"q", [](){ quit(); }};
  } funcs;

public:
  static constexpr unsigned FNS_SIZE = 
    sizeof(private_fns_struct)/sizeof(decltype(private_fns_struct::placeholder));

  const char* get_fn_name(unsigned i) {
    return auto_switch<0, FNS_SIZE>::get(i, funcs).template get<0>();
  }
  auto& get(unsigned i) {
    return auto_switch<0, FNS_SIZE>::get(i, funcs);
  }
  template<typename... Args>
  auto call(unsigned i, Args&&... args) {
    auto fptr = get(i).template get<1>();
    return invoker<decltype(fptr)>::run(fptr, forward<Args>(args)...);
  }
};
} // namespace jos
#endif // SHELL_FUNCS_HPP_
