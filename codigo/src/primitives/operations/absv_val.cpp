#include "../../../headers/primitives/operations.hpp"
#include <cstdint>

float absv (float x) {
  *(uint32_t *)&x &= 0x7FFFFFFF;
  return x;
}
