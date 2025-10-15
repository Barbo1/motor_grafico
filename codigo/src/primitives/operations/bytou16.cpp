#include "../../../headers/primitives/operations.hpp"
#include <cstdint>

uint16_t bytou16 (const uint8_t& p1, const uint8_t& p2) {
  return p1 | (p2 << 8);
}
