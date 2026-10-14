#ifndef MACROS_H_
#define MACROS_H_

namespace jos {
#define WIDTH  (80)
#define HEIGHT (25)
#define SCREEN_SIZE (WIDTH*HEIGHT)
#define DEFAULT_COLOR (0x0F)
#define NULL (0)
#define SHELL_OFFSET (4)
#define DEFAULT_ARG (-1)

extern int cursor_x;
extern int cursor_y;

extern "C" {
unsigned char inb(unsigned short port);
void outb(unsigned char value, unsigned short port);
}
} // namespace jos
#endif // MACROS_H_
