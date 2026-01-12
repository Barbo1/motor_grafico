#include "../../../../headers/primitives/bool_matrix.hpp"

uint64_t BoolMatrixU::get_width () {
  return this->_columns_;
}

uint64_t BoolMatrixU::get_height() {
  return this->_rows_;
}

uint64_t BoolMatrixU::get_lenc () {
  return this->_lenc_;
}

uint64_t BoolMatrixU::get_lenr () {
  return this->_lenr_;
}
