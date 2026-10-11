#ifndef KEYBOARD_H_
#define KEYBOARD_H_

namespace jos {
struct sc_matrix {
  char data[128];
};

void get_csr_pos(int* x, int* y);
unsigned char kbd_rd_sc();
char get_ch_inp();
} // namespace jos
#endif // KEYBOARD_H_
