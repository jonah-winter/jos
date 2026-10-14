#include "macros.hpp"
#include "keyboard.hpp"

namespace jos {
static constexpr sc_matrix scm = []() constexpr {
    sc_matrix matrix{};

    matrix.data[0x02] = '1';
    matrix.data[0x03] = '2';
    matrix.data[0x04] = '3';
    matrix.data[0x05] = '4';
    matrix.data[0x06] = '5';
    matrix.data[0x07] = '6';
    matrix.data[0x08] = '7';
    matrix.data[0x09] = '8';
    matrix.data[0x0A] = '9';
    matrix.data[0x0B] = '0';
    matrix.data[0x0C] = '-';
    matrix.data[0x0D] = '=';
    matrix.data[0x0E] = '\b';
    matrix.data[0x0F] = '\t';

    matrix.data[0x10] = 'q';
    matrix.data[0x11] = 'w';
    matrix.data[0x12] = 'e';
    matrix.data[0x13] = 'r';
    matrix.data[0x14] = 't';
    matrix.data[0x15] = 'y';
    matrix.data[0x16] = 'u';
    matrix.data[0x17] = 'i';
    matrix.data[0x18] = 'o';
    matrix.data[0x19] = 'p';
    matrix.data[0x1A] = '[';
    matrix.data[0x1B] = ']';
    matrix.data[0x1C] = '\n';

    matrix.data[0x1E] = 'a';
    matrix.data[0x1F] = 's';
    matrix.data[0x20] = 'd';
    matrix.data[0x21] = 'f';
    matrix.data[0x22] = 'g';
    matrix.data[0x23] = 'h';
    matrix.data[0x24] = 'j';
    matrix.data[0x25] = 'k';
    matrix.data[0x26] = 'l';
    matrix.data[0x27] = ';';
    matrix.data[0x28] = '\'';
    matrix.data[0x29] = '`';

    matrix.data[0x2B] = '\\';
    matrix.data[0x2C] = 'z';
    matrix.data[0x2D] = 'x';
    matrix.data[0x2E] = 'c';
    matrix.data[0x2F] = 'v';
    matrix.data[0x30] = 'b';
    matrix.data[0x31] = 'n';
    matrix.data[0x32] = 'm';
    matrix.data[0x33] = ',';
    matrix.data[0x34] = '.';
    matrix.data[0x35] = '/';
    matrix.data[0x39] = ' ';

    return matrix;
}();

const char* sc_map = scm.data;

unsigned char kbd_rd_sc() {
  while ((inb(0x64) & 1) == 0) { asm volatile ("pause"); }
  return inb(0x60);
}

char get_ch_inp() {
  unsigned char sc = kbd_rd_sc();
  if (sc & 0x80) { return 0; }
  if (sc >= 128 || sc_map[sc] == '\0') { return 0; }
  return sc_map[sc];
}
} // namespace jos