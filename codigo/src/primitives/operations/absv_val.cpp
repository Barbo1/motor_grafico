#include "../../../headers/primitives/operations.hpp"
#include <cstdint>
#include <bit>

float absv (float x) {
  return std::bit_cast<float>(std::bit_cast<uint32_t>(x) & 0x7FFFFFFF);
}
