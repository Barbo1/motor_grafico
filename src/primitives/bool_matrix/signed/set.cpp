#include "../../../../headers/primitives/bool_matrix.hpp"

void BoolMatrixS::set (uint64_t row, uint64_t column) {
  int r = (row & 7) << 3;
  uint64_t pos = (uint64_t)1 << (r + (column & 7));
  uint64_t& curr = this->_data_ [(row >> 3) * this->_lenc_ + (column >> 3)];
  if ((curr & pos) == 0) {
    this->_parity_ [row >> 3] ^= 255ULL << r;
    curr |= pos;
  }
}
