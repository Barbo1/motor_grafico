#include "../../../../headers/primitives/bool_matrix.hpp"

BoolMatrixU::~BoolMatrixU () {
  delete [] this->_data_;
}
