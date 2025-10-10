#include "../../../headers/primitives/operations.hpp"
#include <cstdint>

void absv (float* x) {
  uint32_t* a = (uint32_t *)x;
  *a &= 0x7FFFFFFF;
}
