#include "../../../headers/primitives/glyph_system.hpp"
#include <cstdint>

bool GlyphsSystem::is_cached (char16_t character, uint16_t size, SDL_Color color) {
  Uint32 colori = ((Uint32)color.r << 24) & ((Uint32)color.g << 16) & ((Uint32)color.b << 8) & (Uint32)color.a;
  uint32_t key = GlyphsSystem::get_key(character, size, colori);
  return this->cached_glyphs.find(key) != this->cached_glyphs.end();
}
