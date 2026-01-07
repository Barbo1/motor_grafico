#include "../../../../headers/primitives/glyph_system.hpp"

bool TTFBoolMatrix::operator() (unsigned row, unsigned column) {
  return _data_[(row >> 3)*_lenc_ + (column >> 3)] & ((uint64_t)1 << (((row & 7) << 3) + (column & 7)));
}
