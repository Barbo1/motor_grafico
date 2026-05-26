#include "../../../../headers/concepts/gui.hpp"
#include <cstdint>
#include <cstring>

std::string TextBox::get_text() const {
  std::string ret(this->text_len, '0');
  for (uint32_t i = 0; i < this->text_len; i++)
    ret[i] = (char)this->text[i];
  return ret;
}

std::u16string TextBox::get_text_16() const {
  std::u16string ret(this->text_len, '0');
  std::memcpy(ret.data(), this->text, sizeof(char16_t) * this->text_len);
  return ret;
}
