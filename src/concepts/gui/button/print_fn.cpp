#include "../../../../headers/concepts/gui.hpp"

void Button::set_selected_fn (std::function<void(Dir2)> fn) {
  this->selected_fn = fn;
}

void Button::set_observed_fn (std::function<void(Dir2)> fn) {
  this->observed_fn = fn;
}

void Button::set_quiet_fn (std::function<void(Dir2)> fn) {
  this->quiet_fn = fn;
}
