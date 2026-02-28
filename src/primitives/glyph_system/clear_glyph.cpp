#include "../../../headers/primitives/glyph_system.hpp"
#include <cstdint>

void GlyphsSystem::clear_glyph (char16_t character, uint32_t size) {
  uint32_t key = GlyphsSystem::get_key(character, size);
  this->cached_glyphs.extract (key);
}
