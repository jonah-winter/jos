#ifndef QUIT_HPP_
#define QUIT_HPP_

namespace jos {
/*enum class QEMU_STATUS : unsigned char {
  QEMU_EXIT_SUCCESS=1,
  QEMU_EXIT_FAILURE=0,
};

inline void quit(QEMU_STATUS status) {
  outb(static_cast<unsigned char>(status), 0x501);
}*/
inline void quit(void) {
  outb(0x00, 0x501);
}
} // namespace jos
#endif // QUIT_HPP_
