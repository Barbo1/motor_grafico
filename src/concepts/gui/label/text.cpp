#include "../../../../headers/concepts/gui.hpp"
#include <cstdint>
#include <string_view>

std::string_view Label::get_text() const {
  return std::string_view(this->text, this->text_len);
}

void Label::set_text(std::string_view str) {
  this->config |= LabelConfig::LCchanged;
  this->text_len = std::min(static_cast<uint32_t>(str.size()), this->max_len);
  for (uint32_t i = 0; i < this->text_len; i++)
    this->text[i] = (char16_t)str[i];
}
