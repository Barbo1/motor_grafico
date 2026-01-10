#include "../../../../headers/primitives/bool_matrix.hpp"
#include <cstdint>

bool BoolMatrixS::operator() (unsigned row, unsigned column) {
  uint64_t dev = (((row & 7) << 3) + (column & 7));
  return (_data_[(row >> 3)*_lenc_ + (column >> 3)] & ((uint64_t)1 << dev)) >> dev;
}
