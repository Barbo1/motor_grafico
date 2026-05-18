#include "../../../../headers/concepts/gui.hpp"

void Slider::set_base_dimentions (const Dir2& dimentions) {
  this->base_dims = dimentions;
}

Dir2 Slider::get_base_dimentions () const {
  return this->base_dims;
}

void Slider::set_sign_dimentions (const Dir2& dimentions) {
  this->sign_dims = dimentions;
}

Dir2 Slider::get_sign_dimentions () const {
  return this->sign_dims;
}
