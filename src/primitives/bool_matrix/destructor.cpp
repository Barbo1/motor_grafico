#include "../../../headers/primitives/bool_matrix.hpp"

BoolMatrix::~BoolMatrix () {
  delete [] this->_data_;
}
