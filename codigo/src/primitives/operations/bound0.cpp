#include "../../../headers/primitives/operations.hpp"
#include <cstdint>
#include <bit>

float bound0 (const float& f, const float& b) {
  uint32_t fi = std::bit_cast<uint32_t>(f), bi = std::bit_cast<uint32_t>(b);
  uint32_t mask = (int32_t)(fi - bi) >> 31;
  mask = ((fi & mask) | (bi & ~mask)) & (int32_t)~fi >> 31;
  return std::bit_cast<float>(mask);
}
