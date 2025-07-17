#pragma once

#include "../../../../headers/concepts/image_processing.hpp"

template <bool LSB = true>
uint64_t read_bytes (uint8_t* vec, uint64_t& pos, uint16_t N) {
  uint64_t ret = 0;
  for (int i = pos, sh = 0; pos < N + pos; i++, sh += 8)
    ret |= vec[i] << sh;
  pos += N;
  return ret;
}
