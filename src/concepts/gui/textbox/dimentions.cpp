#include "../../../../headers/concepts/gui.hpp"

void TextBox::set_dimentions (const Dir2& dimentions) {
  this->dims = dimentions;
}

Dir2 TextBox::get_dimentions () const {
  return this->dims;
}
