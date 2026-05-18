#include "../../../../headers/concepts/gui.hpp"

void Slider::set_base_fn (std::function<void(Dir2)> fn) {
  this->base_fn = fn;
}
