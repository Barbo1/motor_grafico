#include "../../../../headers/concepts/gui.hpp"
#include <cstring>

std::string TextBox::get_text() const {
  std::string ret(this->text_len, '\0');
  std::memcpy(ret.data(), this->text, sizeof(char) * this->text_len);
  return ret;
}

bool TextBox::set_text(const std::string& str) {
  bool could = str.size() <= this->max_len;
  if (could)
    std::memcpy(this->text, str.data(), sizeof(char) * str.size());
  return could;
}
