#include "../../../headers/primitives/operations.hpp"
#include <cstdint>
#include <bit>

float sgn (float a) {
  return std::bit_cast<float>(
    (std::bit_cast<uint32_t>(a) & 0x80000000) | 0x3F800000
  );
}
