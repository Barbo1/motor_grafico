#include "../../../headers/primitives/bool_matrix.hpp"

void BoolMatrixU::set (uint64_t row, uint64_t column) {
  int r = (row & 7) << 3;
  uint64_t pos = (uint64_t)1 << (r + (column & 7));
  this->_data_ [(row >> 3) * this->_lenc_ + (column >> 3)] |= pos;
}
