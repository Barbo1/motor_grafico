#include "../../../../headers/primitives/bool_matrix.hpp"

uint64_t BoolMatrixU::number_bits (uint64_t lenr, uint64_t lenc) {
  return std::popcount(this->_data_[lenr * this->_lenc_ + lenc]);
}
