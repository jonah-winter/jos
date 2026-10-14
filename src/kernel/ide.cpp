#include "ide.hpp"
#include "macros.hpp"

namespace jos {
namespace ide {
static constexpr int PRIMARY_MASTER   = 0xF0;
static constexpr int SECONDARY_MASTER = 0xE0;
static void wait_rdy() {
  volatile int sink = 0;
  while(inb(REG_STATUS) & STATUS_BSY) { sink += 1; }
}
static void wait_drq() {
  volatile int sink = 0;
  while(!(inb(REG_STATUS) & STATUS_DRQ)) { sink += 1; }
}

void sctr_rd(unsigned lba, unsigned char* buffer) {
  wait_rdy();
  outb(PRIMARY_MASTER | ((lba >> 24) & 0x0F), REG_DRIVE_SEL);
  for (volatile unsigned i{0}; i < 1000; i++) { asm volatile ("nop"); }
  wait_rdy();
  outb(1, REG_SEC_COUNT);
  outb((unsigned char)(lba & 0xFF), REG_LBA_LOW);
  outb((unsigned char)((lba >> 8) & 0xFF), REG_LBA_MID);
  outb((unsigned char)((lba >> 16) & 0xFF), REG_LBA_HIGH);
  outb(CMD_READ_PIO, REG_COMMAND);

  wait_rdy();
  wait_drq();
  for (unsigned i{0}; i < 256; i++) {
    unsigned short word;
    asm volatile ("inw %1, %0" : "=a"(word) : "Nd"(REG_DATA));
    buffer[i*2]   = (unsigned char)(word & 0xFF);
    buffer[i*2+1] = (unsigned char)(word >> 8);
  }
}

void sctr_wr(unsigned lba, const unsigned char* buffer) {
  wait_rdy();

  outb(PRIMARY_MASTER | ((lba >> 24) & 0x0F), REG_DRIVE_SEL);
  outb(1, REG_SEC_COUNT);
  outb((unsigned char)(lba & 0xFF),        REG_LBA_LOW);
  outb((unsigned char)((lba >> 8) & 0xFF), REG_LBA_MID);
  outb((unsigned char)((lba >> 16) & 0xFF),REG_LBA_HIGH);

  outb(CMD_WRITE_PIO, REG_COMMAND);

  wait_rdy();
  wait_drq();

  const unsigned short* wrd_bfr = reinterpret_cast<const unsigned short*>(buffer);
  for (int i = 0; i < 256; i++) {
    unsigned short word = 
       static_cast<unsigned short>(buffer[i*2]) |
       (static_cast<unsigned short>(buffer[i*2+1]) << 8);
    asm volatile ("outw %0, %1" : : "a"(word), "Nd"(REG_DATA));
  }

  outb(CMD_CACHE_FLUSH, REG_COMMAND);
  wait_rdy();
}
} // namespace ide
} // namespace jos
