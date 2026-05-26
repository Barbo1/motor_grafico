#include "../../../../headers/concepts/gui.hpp"

Dir2 Label::get_position() const {
  return this->position;
}

void Label::set_position(const Dir2& pos) {
  this->position = pos;
}
