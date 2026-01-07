#include "../../../../headers/primitives/glyph_system.hpp"

void TTFBoolMatrix::set (uint64_t row, uint64_t column) {
  this->_data_ [(row >> 3) * this->_lenc_ + (column >> 3)] |= (uint64_t)1 << (((row & 7) << 3) + (column & 7));
}
