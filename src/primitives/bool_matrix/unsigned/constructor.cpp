#include "../../../../headers/primitives/bool_matrix.hpp"

BoolMatrixU::BoolMatrixU (unsigned rows, unsigned columns) noexcept {
  this->_columns_ = columns;
  this->_rows_ = rows;
  this->_lenr_ = (rows + 7) >> 3;
  this->_lenc_ = (columns + 7) >> 3;
  this->_data_ = new uint64_t [this->_lenr_ * this->_lenc_];
  std::memset (this->_data_, 0, sizeof (uint64_t) * this->_lenr_ * this->_lenc_);
}
