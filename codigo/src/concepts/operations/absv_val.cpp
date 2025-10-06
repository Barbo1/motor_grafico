#include "../../../headers/concepts/operations.hpp"
#include <cstdint>

float absv (float x) {
  uint32_t* a = (uint32_t *)&x;
  *a &= 0x7FFFFFFF;
  return x;
}
