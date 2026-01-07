#include "../../../../headers/primitives/glyph_system.hpp"

void TTFBoolMatrix::fill_bounds () {
  for (uint64_t r = 0; r < this->_lenr_; r++) {
    uint64_t curr_vec = 0;
    for (uint64_t c = 0; c < this->_lenc_; c++) {
      uint64_t curr = this->_data_ [r * this->_lenc_ + c];
      curr |= (curr_vec ^= curr & 0x0101010101010101);
      curr_vec <<= 1;
      curr |= (curr_vec ^= curr & 0x0202020202020202);
      curr_vec <<= 1;
      curr |= (curr_vec ^= curr & 0x0404040404040404);
      curr_vec <<= 1;
      curr |= (curr_vec ^= curr & 0x0808080808080808);
      curr_vec <<= 1;
      curr |= (curr_vec ^= curr & 0x1010101010101010);
      curr_vec <<= 1;
      curr |= (curr_vec ^= curr & 0x2020202020202020);
      curr_vec <<= 1;
      curr |= (curr_vec ^= curr & 0x4040404040404040);
      curr_vec <<= 1;
      this->_data_ [r * this->_lenc_ + c] = curr | (curr_vec ^= curr & 0x8080808080808080);
      curr_vec >>= 7;
    }
  }
}
