#include "../../../headers/primitives/glyph_system.hpp"
#include <cstdint>

void GlyphsSystem::cache (char16_t character, uint16_t size, SDL_Color color) {
  uint32_t key = GlyphsSystem::get_key(
   character, 
   size, 
   ((Uint32)color.r << 24) & ((Uint32)color.g << 16) & ((Uint32)color.b << 8) & (Uint32)color.a
  );

  if (this->cached_glyphs.find(key) == this->cached_glyphs.end() && this->is_meta) {
    SDL_Surface* sur = this->raster (character, size, color);
    this->cached_glyphs.insert ({key, 
      TTFCachedGlyphInfo {
        SDL_CreateTextureFromSurface(glb->get_render(), sur), 
        (uint32_t)sur->w, 
        (uint32_t)sur->h
      }
    });
    SDL_FreeSurface (sur);
  }
}
