#include "memory.hpp"

extern "C" void* memset(void* dest, int val, unsigned count) {
  unsigned char* ptr = static_cast<unsigned char*>(dest);
  for (unsigned int i = 0; i < count; i++) {
    ptr[i] = static_cast<unsigned char>(val);
  }
  return dest;
}
