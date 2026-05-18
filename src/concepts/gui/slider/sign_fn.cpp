#include "../../../../headers/concepts/gui.hpp"

void Slider::set_sign_fn (std::function<void(Dir2)> fn) {
  this->sign_fn = fn;
}
