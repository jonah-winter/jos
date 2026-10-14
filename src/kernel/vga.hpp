#ifndef VGA_H_
#define VGA_H_

namespace jos {
extern volatile unsigned char* const VGA;
static constexpr const char PROMPT[] = "jsh>";

void vga_ld(char ch, char color);
void clr(void);
void get_csr_pos(int* x, int* y);
void ld_ch(char ch);
void ld_str(const char* str);
void bsp(void);
void safe_bsp(void);
void set_csr_pos(int x, int y);
void dsbl_csr(void);
void enbl_csr(void);
void scroll_down(void);
void ld_hex_byte(unsigned char byte);
} // namespace jos
#endif // VGA_H_
