#ifndef SHELL_CMD_IMPLS_HPP_
#define SHELL_CMD_IMPLS_HPP_

#include "shell/shl_util.hpp"
#include "ide.hpp"

namespace jos {
struct shell_funcs;
inline void readsec_cmd_impl(parse_args& args) {
  int sector = jstd::cvt_arg<int>(args.get_arg(0));
  if (sector < 0) {
    ld_str("Invalid sector\n");
    return;
  }
  int start  = jstd::cvt_arg<int>(args.get_arg(1));
  int end    = jstd::cvt_arg<int>(args.get_arg(2));
  if (start == -1) start = 0; 
  if (end == -1) end = 512; 
  if (start < 0) start = 0; 
  if (end > 512) end = 512; 
  if (end <= start) end = 512; 
  static unsigned char buffer[512]; 
  unsigned cols{0}; 
  ide::sctr_rd(sector, buffer); 
  for(int i{start}; i < end; i++) { 
    if(cols > 0 && (cols & 15) == 0) { ld_ch('\n'); } 
    jstd::ld_hex(buffer[i]); 
    ld_ch(' '); 
    cols++; 
  } 
  ld_ch('\n');
}

inline void writesec_cmd_impl(parse_args& args) {
  const char* data = args.get_arg(0);
  if(!data[0]) { 
    return;
  }
  int sector = jstd::cvt_arg<int>(args.get_arg(1));
  if(sector == -1) { sector = 0; }
  else if(sector < 0) { ld_str("invalid sector\n"); return; }
  static unsigned char buffer[512] = {};
  unsigned i{0};
  while(data[i] != '\0' && i < 512) {
    buffer[i] = static_cast<unsigned char>(data[i]);
    i++;
  }
  ide::sctr_wr(static_cast<unsigned>(sector), buffer);
}

inline void help_cmd_impl(shell_funcs& sf);
} // namespace jos
#endif // SHELL_CMD_IMPLS_HPP_
