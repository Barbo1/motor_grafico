#include "../../../headers/primitives/operations.hpp"
#include <cstdint>

uint64_t bytou64 (
  const uint8_t& p1, const uint8_t& p2, const uint8_t& p3, const uint8_t& p4,
  const uint8_t& p5, const uint8_t& p6, const uint8_t& p7, const uint8_t& p8
) {
  return p1 | (p2 << 8) | (p3 << 16) | (p4 << 24) | ((uint64_t)p5 << 32) | 
    ((uint64_t)p6 << 40) | ((uint64_t)p7 << 48) | ((uint64_t)p8 << 56);
}
