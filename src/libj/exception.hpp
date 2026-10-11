#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

namespace jstd {
struct exception {
protected:
  char buffer[128];
public:
  exception(const char* m) {
    strcpy(this->buffer, m, 128);
  }
  constexpr const char* what() const noexcept {
    return this->buffer;
  }
};

struct index_out_of_bounds_except_type : public exception {
  static constexpr const char* dflt_msg = "index out of bounds error: ";
  static constexpr unsigned dflt_sz = sizeof(dflt_msg)/sizeof(dflt_msg[0]);
  index_out_of_bounds_except_type(const char* m) 
    : exception(dflt_msg)
  {
    strcpy(this->buffer+dflt_sz-1, m, 128-dflt_sz);
  }
};

template<typename Exception>
requires(__is_base_of(exception, Exception))
void k_throw(const Exception& e) {
  clr();
  vga_ld('\n', DEFAULT_COLOR);
  ld_str(e.what());
  while(1){};
}

void index_out_of_bounds(const char* msg) {
  k_throw(index_out_of_bounds_except_type{msg});
}
} // namespace jstd
#endif // EXCEPTION_HPP_
