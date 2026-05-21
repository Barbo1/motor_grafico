#include "../../../../headers/concepts/gui.hpp"

bool TextBox::get_active () const {
  return this->config & 1;
}
