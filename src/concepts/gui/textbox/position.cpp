#include "../../../../headers/concepts/gui.hpp"

Dir2 TextBox::get_position() const {
  return this->position;
}

void TextBox::set_position(const Dir2& pos) {
  this->position = pos;
}
