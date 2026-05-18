#include "../../../../headers/concepts/gui.hpp"

void CheckBox::set_active_fn (std::function<void(Dir2)> fn) {
  this->active_fn = fn;
}

void CheckBox::set_deactive_fn (std::function<void(Dir2)> fn) {
  this->deactive_fn = fn;
}
