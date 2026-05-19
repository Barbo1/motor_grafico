#include "../../../../headers/concepts/gui.hpp"

void TextBox::set_background_fn (std::function<void(Dir2)> fn) {
  this->background = fn;
}
