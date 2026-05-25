#include "../../../headers/concepts/glyph_system.hpp"
#include <cstdint>

GlyphsSystem::~GlyphsSystem () {
  std::map<uint32_t, TTFCachedGlyphInfo>::iterator iter = this->cached_glyphs.begin();
  while (iter != this->cached_glyphs.end()) {
    SDL_DestroyTexture(iter->second.tex);
    iter++;
  }
}
