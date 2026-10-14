#ifndef IDE_HPP_
#define IDE_HPP_

//#include "libj/type_traits.hpp"

namespace jos {
namespace ide {
constexpr unsigned short REG_DATA       = 0x1F0;
constexpr unsigned short REG_SEC_COUNT  = 0x1F2;
constexpr unsigned short REG_LBA_LOW    = 0x1F3;
constexpr unsigned short REG_LBA_MID    = 0x1F4;
constexpr unsigned short REG_LBA_HIGH   = 0x1F5;
constexpr unsigned short REG_DRIVE_SEL  = 0x1F6;
constexpr unsigned short REG_COMMAND    = 0x1F7;
constexpr unsigned short REG_STATUS     = 0x1F7;

constexpr unsigned char STATUS_BSY      = 0x80;
constexpr unsigned char STATUS_DRQ      = 0x08;
constexpr unsigned char STATUS_ERR      = 0x01;

constexpr unsigned char CMD_READ_PIO    = 0x20;
constexpr unsigned char CMD_WRITE_PIO   = 0x30;
constexpr unsigned char CMD_CACHE_FLUSH = 0xE7;

void sctr_rd(unsigned lba, unsigned char* buffer);
void sctr_wr(unsigned lba, const unsigned char* buffer);
} // namespace ide
} // namespace jos
#endif // IDE_HPP_
