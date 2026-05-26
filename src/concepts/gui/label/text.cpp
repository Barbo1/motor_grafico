#include "../../../../headers/concepts/gui.hpp"
#include <cstdint>
#include <cstring>

std::string Label::get_text() const {
  std::string ret(this->text_len, '0');
  for (uint32_t i = 0; i < this->text_len; i++)
    ret[i] = (char)this->text[i];
  return ret;
}

std::u16string Label::get_text_16() const {
  std::u16string ret(this->text_len, '0');
  std::memcpy(ret.data(), this->text, sizeof(char16_t) * this->text_len);
  return ret;
}

void Label::set_text(std::string_view str) {
  this->config |= LabelConfig::LCchanged;
  this->text_len = std::min(static_cast<uint32_t>(str.size()), this->max_len);
  for (uint32_t i = 0; i < this->text_len; i++)
    this->text[i] = (char16_t)str[i];
}

void Label::set_text(std::u16string_view str) {
  this->config |= LabelConfig::LCchanged;
  this->text_len = std::min(static_cast<uint32_t>(str.size()), this->max_len);
  std::memcpy(
    this->text, 
    str.data(), 
    sizeof(char16_t) * this->text_len
  );
}
