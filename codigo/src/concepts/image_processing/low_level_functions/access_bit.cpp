#pragma once

#include "./reverse_bits.cpp"

/* Function to read the nexts N bits of an array of bits that follows the RFC-1951 specification. 
 * */
template<bool LSB = true>
uint64_t access_bit (uint8_t* vec, uint64_t& pos, uint16_t N) {
  uint64_t ret = 0;
  uint64_t mod_pos = pos & 7;
  int vec_pos = pos >> 3;
  pos += N;

  int aux = mod_pos + N;
  uint64_t mask = (1 << aux) - (1 << mod_pos);
  uint8_t* mask8 = (uint8_t *)&mask;

  for (int sh = 0; sh < aux; sh += 8)
    ret |= ((*(mask8++) & vec [vec_pos++]) << sh) >> mod_pos;

  if constexpr (!LSB)
    ret = reverse_bits (ret, N);
  return ret;
}
