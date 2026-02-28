#include "../../../headers/primitives/glyph_system.hpp"
#include <cstdint>

uint32_t GlyphsSystem::get_key(char16_t character, uint32_t size) {
  return (size && character ? size << 16 | (uint32_t)character : 0);
}
