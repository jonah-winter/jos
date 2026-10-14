#include "macros.hpp"
#include "vga.hpp"
#include "keyboard.hpp"
#include "shell/shl_fns.hpp"
#include "libj/memory.hpp"

constexpr int CMDS_SIZE = 100;

extern "C" void k_main(void) {
  using namespace jos;
  clr();
  enbl_csr();
  ld_str("jsh>");
  set_csr_pos(4,0);
  shell_funcs sf;

#define PROMPT_SIZE (76)
#define ARG_SIZE (128)
  // currently pressed
  char cp[PROMPT_SIZE] = {0};
  int i{0};
  bool found = false;
  while(1) {
    char c = get_ch_inp();
    if(c) {
      ld_ch(c);
      if(c == '\n') {
        cp[i] = '\0';
        for(int j{0}; j < shell_funcs::FNS_SIZE; j++) {
          const char* name = sf.get_fn_name(j);
          int len = jstd::strlen(name);
          if(jstd::strncmp(name, cp, len) == 0 && 
            (cp[len] == '\0' || cp[len] == ' ')) {
            int offset = len;
            if (cp[len] == ' ') {
              offset++;
            }
            parse_args args = parse_args(cp+offset);
            sf.call(j, args);
            found = true;
          }
        }
        if(!found && i > 0) { ld_str("Unknown command\n"); }
        memset(cp, 0, PROMPT_SIZE); 
        i = 0;
        ld_str(PROMPT);
      } else if(i < PROMPT_SIZE-1) {
        cp[i] = c;
        ++i;
      }
      int x, y;
      get_csr_pos(&x, &y);
      set_csr_pos(x, y);
    }
    found = false;
  }
}
