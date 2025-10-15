#include "../../../headers/primitives/operations.hpp"
#include <cstdint>

uint32_t bytou32 (const uint8_t& p1, const uint8_t& p2, const uint8_t& p3, const uint8_t& p4) {
  return p1 | (p2 << 8) | (p3 << 16) | (p4 << 24);
}
