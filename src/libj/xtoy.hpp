#ifndef X_TO_Y_HPP_
#define X_TO_Y_HPP_

namespace jstd {
inline int atoi(const char* str) {
    int res{0};
    int sign{1};
    int i{0};
    if (str[0] == '-') {
      sign = -1;
      i++;
    }
    while(str[i] >= '0' && str[i] <= '9') {
      res = res*10+(str[i]-'0');
      i++;
    }
    return sign*res;
}

template<typename T>
inline T cvt_arg(const char* arg);
template<>
inline int cvt_arg<int>(const char* arg) {
  if (!arg || arg[0] == '\0') { return -1; }
  return atoi(arg);
}
template<>
inline unsigned cvt_arg<unsigned>(const char* arg) {
  return static_cast<unsigned>(atoi(arg));
}
template<>
inline const char* cvt_arg<const char*>(const char* arg) {
  return arg;
}
template<>
inline char cvt_arg<char>(const char* arg) {
  return arg[0];
}
}
#endif // X_TO_Y_HPP_
