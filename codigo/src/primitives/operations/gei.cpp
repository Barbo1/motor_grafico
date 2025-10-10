#include "../../../headers/primitives/operations.hpp"
#include <cstdint>
#include <bit>

int gei (const float& x, const float& y) {
  return (std::bit_cast<uint32_t>(x - y) ^ 0x80000000) >> 31;
}
