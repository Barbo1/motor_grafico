#pragma once

#include <cstdint>

/* Function to read the nexts N bits, used in the PNG algorithm.
 * It assumes that N < 16.
 * */
uint64_t access_bit_png (const uint8_t* vec, uint64_t& pos, uint64_t N) {
  uint64_t arr_pos = pos >> 3;
  uint64_t mod_pos = 24 - (pos & 7);
  uint64_t arr_res = mod_pos - N;
  pos += N;

  uint64_t ret = vec[arr_pos] << 16 | vec[arr_pos + 1] << 8 | vec[arr_pos + 2];
  uint64_t mask = (1 << mod_pos) - (1 << arr_res);
  return (ret & mask) >> arr_res;
}


/* Function to read the nexts N bits of an array of bits that follows the RFC-1951 specification.
 * It assumes that N < 16.
 * */
uint64_t access_one_bit (const uint8_t* vec, uint64_t& pos) {
  uint64_t ret = vec[pos >> 3] & (1 << (pos & 7));
  pos++;
  return ret;
}

/* Function to read the nexts N bits of an array of bits that follows the RFC-1951 specification.
 * It assumes that N < 16.
 * */
template<bool LSB = true>
uint64_t access_bit (const uint8_t* vec, uint64_t& pos, uint16_t N) {
  uint64_t mod_pos = pos & 7;
  uint64_t arr_pos = pos >> 3;
  pos += N;

  uint64_t ret = vec[arr_pos] | 
    vec[arr_pos + 1] << 8 | 
    vec[arr_pos + 2] << 16;
  return (ret >> mod_pos) & ((1 << N) - 1);
}

/* Function to read the nexts N bits of an array of bits that follows the RFC-1951 specification 
 * in LSB order. It assumes that N < 8.
 * */
template<>
uint64_t access_bit<false> (const uint8_t* vec, uint64_t& pos, uint16_t N) {
  uint64_t arr_pos = pos >> 3;
  uint64_t mod_pos = 23 - (pos & 7);
  uint64_t arr_res = mod_pos - N;
  pos += N;

  uint64_t ret = vec[arr_pos] << 8 | vec[arr_pos + 1];
  ret = (0x0F0F & ret) << 8 | (0xF0F0 & ret);
  ret = (0x33333333 & ret) << 4 | (0xCCCCCCCC & ret);
  ret = (0x55555555 & ret) << 2 | (0xAAAAAAAA & ret);
  uint64_t mask = (1 << mod_pos) - (1 << arr_res);
  return (ret & mask) >> arr_res;
}
