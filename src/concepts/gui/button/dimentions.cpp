#include "../../../../headers/concepts/gui.hpp"

void Button::set_dimentions (const Dir2& dimentions) {
  this->dims = dimentions;
}

Dir2 Button::get_dimentions () const {
  return this->dims;
}
