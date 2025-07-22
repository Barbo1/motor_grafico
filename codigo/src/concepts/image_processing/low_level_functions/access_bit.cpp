#pragma once

#include "./reverse_bits.cpp"
#include <cstdint>

/* Function to read the nexts N bits of an array of bits that follows the RFC-1951 specification. 
 * */
template<bool LSB = true>
uint64_t access_bit (const uint8_t* vec, uint64_t& pos, uint16_t N) {
  uint64_t mod_pos = pos & 7;
  uint64_t arr_pos = pos >> 3;

  uint64_t mask = ((1 << (mod_pos + N)) - (1 << mod_pos));
  uint64_t ret = ((
      vec[arr_pos] | 
      vec[arr_pos + 1] << 8 | 
      vec[arr_pos + 2] << 16 | 
      vec[arr_pos + 3] << 24
    ) & mask) >> mod_pos;
  pos += N;

  if constexpr (!LSB)
    ret = reverse_bits (ret, N);
  return ret;
}
