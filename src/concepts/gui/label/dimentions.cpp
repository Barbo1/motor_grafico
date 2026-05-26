#include "../../../../headers/concepts/gui.hpp"

void Label::set_dimentions (const Dir2& dimentions) {
  this->dims = dimentions;
}

Dir2 Label::get_dimentions () const {
  return this->dims;
}
