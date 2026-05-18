#include "../../../../headers/concepts/gui.hpp"

void Button::set_position (const Dir2& pos) {
  this->position = pos;
}

Dir2 Button::get_position () const {
  return this->position;
}
