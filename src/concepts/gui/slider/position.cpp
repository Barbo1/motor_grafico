#include "../../../../headers/concepts/gui.hpp"

void Slider::set_position (const Dir2& pos) {
  this->position = pos;
}

Dir2 Slider::get_position () const {
  return this->position;
}
