#include "../../../headers/primitives/operations.hpp"
#include <cstdint>

float sgn (float a) {
  uint32_t* b = (uint32_t*)&a;
  *b &= 0x80000000;
  *b |= 0x3F800000;
  return a;
}
