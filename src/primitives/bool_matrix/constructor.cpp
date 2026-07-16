#include "../../../headers/primitives/bool_matrix.hpp"
#include <cstdint>

BoolMatrix::BoolMatrix (unsigned rows, unsigned columns, Arena& arena, int* error) noexcept {
  this->_columns_ = columns;
  this->_rows_ = rows;
  this->_lenr_ = (rows + 7) >> 3;
  this->_lenc_ = (columns + 7) >> 3;
  this->_data_ = arena.atalloc<uint64_t>(this->_lenr_ * this->_lenc_);
  if (this->_data_ == nullptr) {
    *error = -1;
  } else {
    *error = 0;
    std::memset (this->_data_, 0, sizeof (uint64_t) * this->_lenr_ * this->_lenc_);
  }
}
