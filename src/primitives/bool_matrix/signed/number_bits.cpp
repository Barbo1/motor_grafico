#include "../../../../headers/primitives/bool_matrix.hpp"

uint64_t BoolMatrixS::number_bits (uint64_t lenr, uint64_t lenc) {
  uint64_t ret = this->_data_[lenr * this->_lenc_ + lenc];
  ret = (ret & 0x5555555555555555) + ((ret & 0xAAAAAAAAAAAAAAAA) >> 1);
  ret = (ret & 0x3333333333333333) + ((ret & 0xCCCCCCCCCCCCCCCC) >> 2);
  ret = (ret & 0x0F0F0F0F0F0F0F0F) + ((ret & 0xF0F0F0F0F0F0F0F0) >> 4);
  ret = (ret & 0x00FF00FF00FF00FF) + ((ret & 0xFF00FF00FF00FF00) >> 8);
  ret = (ret & 0x0000FFFF0000FFFF) + ((ret & 0xFFFF0000FFFF0000) >> 16);
  return (ret & 0x00000000FFFFFFFF) + ((ret & 0xFFFFFFFF00000000) >> 32);
}
