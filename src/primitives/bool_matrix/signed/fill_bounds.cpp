#include "../../../../headers/primitives/bool_matrix.hpp"
#include <cstdint>

// Fill the bounds between the pairs of elements that represent the pixels of an image.
// To properly represent an image, the number of bits on in each row must be even.
// 
//      ---------------------            ---------------------    
//      *                   *            *                   *
//   p1 *                   * p2      p1 ********************* p2
//      *                   *            *                   *
//      *                   *     =>     *                   *    
//      *                   *            *                   *
//      *                   *            *                   *    
//      ---------------------            ---------------------    
void BoolMatrixS::fill_bounds () {
  for (uint64_t r = 0; r < this->_lenr_; r++) {
    uint64_t curr_vec = 0;
    for (uint64_t c = 0; c < this->_lenc_; c++) {
      uint64_t& curr = this->_data_ [r * this->_lenc_ + c];
      uint64_t q = curr;
      uint64_t pos = 0x0101010101010101;
      for (uint64_t i = 0; i < 7; i++) {
        curr_vec ^= curr & pos;
        curr |= curr_vec;
        curr_vec <<= 1;
        pos <<= 1;
      }

      curr_vec ^= curr & pos;
      curr |= curr_vec;
      curr &= ((curr | curr_vec) & this->_parity_ [r]) | (q & ~this->_parity_ [r]);
      curr_vec >>= 7;
    }
  }
}
