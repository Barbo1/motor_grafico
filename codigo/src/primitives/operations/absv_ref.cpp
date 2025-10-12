#include "../../../headers/primitives/operations.hpp"
#include <cstdint>

void absv (float* x) {
  *(uint32_t *)x &= 0x7FFFFFFF;
}
