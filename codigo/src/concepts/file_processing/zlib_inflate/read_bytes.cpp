#pragma once

#include <cstdint>

template <bool LSB = true>
uint64_t read_bytes (uint8_t* vec, uint64_t& pos, uint16_t N) {
  uint64_t ret = 0;
  ret = vec[pos];
  for (int i = pos + 1; i < N + pos; i++)
    ret = ret << 8 | vec[i];
  pos += N;
  return ret;
}
