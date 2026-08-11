#include "../../../../headers/concepts/gui.hpp"
#include <cstring>
#include <string_view>

std::string_view TextBox::get_text() const {
  return std::string_view(this->text, static_cast<std::size_t>(this->text_len));
}
