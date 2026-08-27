#include "../../../headers/primitives/bool_matrix.hpp"

void BoolMatrix::set_zeros() {
  std::memset (this->_data_, 0, sizeof (uint64_t) * this->_lenr_ * this->_lenc_);
}
