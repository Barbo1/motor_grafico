#include "../../../../headers/primitives/bool_matrix.hpp"

BoolMatrixS::~BoolMatrixS () {
  delete [] this->_data_;
  delete [] this->_parity_;
}
