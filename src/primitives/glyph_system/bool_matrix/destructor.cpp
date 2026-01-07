#include "../../../../headers/primitives/glyph_system.hpp"

TTFBoolMatrix::~TTFBoolMatrix () {
  delete [] this->_data_;
}
