#include "../../../headers/primitives/operations.hpp"
#include <cstdint>
#include <bit>

int gti (const float& x, const float& y) {
  return std::bit_cast<uint32_t>(y - x) >> 31;
}
