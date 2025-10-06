#include "../../../headers/concepts/operations.hpp"
#include <cstdint>

float max0 (float a) {
  uint32_t* c = (uint32_t *)&a;
  *c &= ~((int32_t)(*c & 0x80000000) >> 31);
  return a;
};
