#include "../../../../headers/concepts/gui.hpp"

void CheckBox::set_dimentions (const Dir2& dimentions) {
  this->dims = dimentions;
}

Dir2 CheckBox::get_dimentions () const {
  return this->dims;
}
