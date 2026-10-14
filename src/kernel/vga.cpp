#include "macros.hpp"
#include "vga.hpp"

namespace jos {
volatile unsigned char* const VGA = (volatile unsigned char*)0xB8000;
int cursor_x = 0;
int cursor_y = 0;

void vga_ld(char ch, char color) {
  unsigned short pos = (cursor_y*WIDTH+cursor_x)*2;
  VGA[pos]   = ch;
  VGA[pos+1] = color;
  ++cursor_x;
  if(cursor_x==WIDTH) { 
    cursor_x = 0; 
    ++cursor_y; 
  }
}

void clr(void) {
  for(int i=0; i < SCREEN_SIZE*2; i+=2) {
    VGA[i]   = ' ';
    VGA[i+1] = DEFAULT_COLOR;
  }
  cursor_x = 0;
  cursor_y = 0;
}

void get_csr_pos(int* x, int* y) {
  if(x!=NULL && y!=NULL) { 
    *x = cursor_x;
    *y = cursor_y;
  }
}

void bsp(void) {
  if(cursor_x>0) {
    --cursor_x;
  } else if(cursor_y>0) {
    --cursor_y;
    cursor_x = WIDTH-1;
  } else {
    return;
  }
  unsigned int pos = (cursor_y*WIDTH+cursor_x)*2;
  VGA[pos]   = ' ';
  VGA[pos+1] = DEFAULT_COLOR;
}

void safe_bsp(void) {
  int x;
  int y;
  get_csr_pos(&x, &y);
  if(x>SHELL_OFFSET) { bsp(); }
}

void set_csr_pos(int x, int y) {
  if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) { return; }
  cursor_x = x;
  cursor_y = y;
  unsigned short pos = y*WIDTH+x;
  outb(0x0E, 0x3D4);
  outb((unsigned char)(pos >> 8), 0x3D5);
  outb(0x0F, 0x3D4);
  outb((unsigned char)(pos & 0xFF), 0x3D5);
}

void ld_ch(char ch) {
  if(ch == '\b') {
    safe_bsp();
    return;
  } else if(ch == '\n') {
    ++cursor_y;
    cursor_x = 0;
    int i = 0;
    if(cursor_y >= HEIGHT) {
      scroll_down();
    }
  } else {
    vga_ld(ch, DEFAULT_COLOR);
  }
}

void ld_str(const char* str) {
  int i = 0;
  while(str[i] != '\0') {
    ld_ch(str[i]);
    ++i;
  }
}

void dsbl_csr(void) {
  outb(0x0A, 0x3D4);
	outb(0x20, 0x3D5);
}

void enbl_csr(void) {
  outb(0x0A, 0x3D4);
  outb(0x0E, 0x3D5);
  outb(0x0B, 0x3D4);
  outb(0x0F, 0x3D5);
}

void scroll_down(void) {
  for(int i=1; i<HEIGHT; i++) {
    for(int j=0; j<WIDTH; j++) {
      unsigned int og_pos   = (i*WIDTH+j)*2;
      unsigned int dest_pos = ((i-1)*WIDTH+j)*2;
      VGA[dest_pos]   = VGA[og_pos];
      VGA[dest_pos+1] = VGA[og_pos+1];
    }
  }
  unsigned row_beg = ((HEIGHT-1)*WIDTH)*2;
  unsigned row_end = (HEIGHT*WIDTH)*2;
  for(unsigned i{row_beg}; i<row_end; i+=2) {
    VGA[i]   = ' ';
    VGA[i+1] = DEFAULT_COLOR;
  }
  cursor_y = HEIGHT-1;
}

void ld_hex_byte(unsigned char byte) {
  const char hex_chars[] = "0123456789ABCDEF";
  unsigned char lo = (byte >> 4) & 0x0F;
  unsigned char hi = byte & 0x0F;
  char str[5];
  str[0] = '0';
  str[1] = 'x';
  str[2] = hex_chars[hi];
  str[3] = hex_chars[lo];
  str[4] = '\0';
  jos::ld_str(str);
}
} // namespace jos
