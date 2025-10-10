#include "../../../headers/primitives/operations.hpp"
#include <cstdint>

float min0 (float num) {
  uint32_t* c = (uint32_t *)&num;
  *c &= ((int32_t)(*c & 0x80000000) >> 31);
  return num;
}

float min (const float& num1, const float& num2) {
  return min0(num1 - num2) + num2;
};
