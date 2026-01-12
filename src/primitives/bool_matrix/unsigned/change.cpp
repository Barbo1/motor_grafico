#include "../../../../headers/primitives/bool_matrix.hpp"
#include <cstdint>

void BoolMatrixU::change (uint64_t row, uint64_t column, uint64_t bit) {
  uint64_t dev = (((row & 7) << 3) + (column & 7));
  uint64_t& curr = this->_data_[(row >> 3)*this->_lenc_ + (column >> 3)];
  curr = (curr & ~(1ULL << dev)) | (bit << dev);
}

