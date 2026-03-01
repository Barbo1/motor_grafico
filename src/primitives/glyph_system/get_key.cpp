#include "../../../headers/primitives/glyph_system.hpp"
#include <cstdint>

uint64_t GlyphsSystem::get_key(char16_t character, uint16_t size, Uint32 color) {
  return (
    size && character ? 
    ((uint64_t)color << 32) | ((uint64_t)size << 16) | (uint64_t)character 
    : 0
  );
}
