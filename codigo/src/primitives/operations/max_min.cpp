#include "../../../headers/primitives/operations.hpp"
#include <cstdint>
#include <bit>

void max_min (const float& num1, const float& num2, const float& res1, const float& res2, float* max, float* min) {
  uint32_t c = ((int32_t)(std::bit_cast<uint32_t>(num1 - num2) & 0x80000000) >> 31), d = ~c;
  uint32_t r1 = std::bit_cast<uint32_t>(res1), r2 = std::bit_cast<uint32_t>(res2);
  *max = std::bit_cast<float>((r1 & d) | (r2 & c));
  *min = std::bit_cast<float>((r1 & c) | (r2 & d));
}
