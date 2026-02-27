#include "../../../headers/primitives/bool_matrix.hpp"
#include <cstdint>

uint64_t BoolMatrixU::operator() (unsigned row, unsigned column) {
  uint64_t dev = (((row & 7) << 3) + (column & 7));
  return (_data_[(row >> 3)*_lenc_ + (column >> 3)] & (1ULL << dev)) >> dev;
}
