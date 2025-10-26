#include "../../../headers/primitives/operations.hpp"
#include <cstdint>
#include <bit>

float max0 (float num) {
  uint32_t c = std::bit_cast<uint32_t>(num);
  c &= ~((int32_t)(c & 0x80000000) >> 31);
  return std::bit_cast<float>(c);
};

float max (const float& num1, const float& num2) {
  return max0(num1 - num2) + num2;
};
