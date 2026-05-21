#include "../../../../headers/concepts/gui.hpp"

bool TextBox::set_cursor(uint32_t pos) {
  bool could = pos < this->max_len;
  if (could)
    this->curr_pos = pos;
  return could;
}

uint32_t TextBox::get_cursor() const {
  return this->curr_pos;
}
