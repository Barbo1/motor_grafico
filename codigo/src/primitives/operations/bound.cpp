#include "../../../headers/primitives/operations.hpp"
#include <cstdint>
#include <bit>

float bound (const float& f, const float& b) {
  uint32_t fi = std::bit_cast<uint32_t>(f), bi = std::bit_cast<uint32_t>(b);
  uint32_t mask = (fi & 0x7FFFFFFF) - bi;
  mask = (int32_t)(mask & 0x80000000) >> 31;
  mask = (fi & mask) | (bi & ~mask) | (fi & 0x80000000);
  return std::bit_cast<float>(mask);
}
