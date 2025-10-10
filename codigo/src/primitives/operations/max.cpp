#include "../../../headers/primitives/operations.hpp"
#include <cstdint>

float max0 (float num1) {
  uint32_t* c = (uint32_t *)&num1;
  *c &= ~((int32_t)(*c & 0x80000000) >> 31);
  return num1;
};

float max (const float& num1, const float& num2) {
  return max0(num1 - num2) + num2;
};
