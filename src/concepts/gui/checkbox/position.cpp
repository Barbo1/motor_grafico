#include "../../../../headers/concepts/gui.hpp"

void CheckBox::set_position (const Dir2& pos) {
  this->position = pos;
}

Dir2 CheckBox::get_position () const {
  return this->position;
}
