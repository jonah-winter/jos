#ifndef UTIL_H_
#define UTIL_H_

namespace jstd {
inline constexpr int strlen(const char* str) {
  int len{0};
  while(str[len] != '\0') { len++; }
  return len;
}
inline constexpr int strncmp(const char* s1, const char* s2, int n) {
  if(n==0) { return 0; }
  while(n>1 && *s1 && *s1==*s2) {
    ++s1;
    ++s2;
    n--;
  }
  return static_cast<unsigned char>(*s1) - static_cast<unsigned char>(*s2);
}

inline constexpr int strcmp(const char* s1, const char* s2) {
  while(*s1 && *s1==*s2) {
    ++s1;
    ++s2;
  }
  return static_cast<unsigned char>(*s1) - static_cast<unsigned char>(*s2);
}

inline constexpr void strncpy(char* dest, const char* str, unsigned len) {
  unsigned i=0;
  while(str[i]!='\0' && i<(len-1)) {
    dest[i] = str[i];
    ++i;
  }
  dest[i] = '\0';
}

inline constexpr void strcat(char* dest, const char* str, unsigned len) {
  unsigned i=0;
  while(dest[i]!='\0' && i<(len-1)) {
    i++;
  }
  unsigned j=0;
  while(str[i]!='\0' && (i+j)<(len-1)) {
    dest[i+j] = str[j];
    ++j;
  }
  dest[i+j] = '\0';
}

inline void ld_hex(unsigned char byte) {
  static const char hex[] = "0123456789ABCDEF";
  char res[3];
  res[0] = hex[(byte>>4) & 0x0F];
  res[1] = hex[byte & 0x0F];
  res[2] = '\0';
  jos::ld_str(res);
}
} // namespace jstd
#endif // UTIL_H_
