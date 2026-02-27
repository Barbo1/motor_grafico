#include "../../../headers/primitives/bool_matrix.hpp"
#include <bit>

uint64_t BoolMatrixU::number_bits_quad (uint64_t lenr, uint64_t lenc) {
  int init_pos = lenr * this->_lenc_ + lenc;
  return 
    std::popcount(this->_data_[init_pos]) + 
    std::popcount(this->_data_[init_pos + 1]) + 
    std::popcount(this->_data_[init_pos + this->_lenc_]) + 
    std::popcount(this->_data_[init_pos + this->_lenc_ + 1]);
}
