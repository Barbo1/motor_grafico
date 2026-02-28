#include "../../../headers/primitives/glyph_system.hpp"
#include <cstdint>

void GlyphsSystem::print (char16_t character, uint32_t size, Dir2 position) {
  uint32_t w, h;
  SDL_Texture* tex = nullptr;

  uint32_t key = GlyphsSystem::get_key(character, size);
  auto founded = this->cached_glyphs.find(key);

  if (founded != this->cached_glyphs.end()) {
    tex = founded->second.tex;
    w = founded->second.w;
    h = founded->second.h;
  } else if (this->is_meta) {
    SDL_Surface* sur = this->raster (character, size);

    tex = SDL_CreateTextureFromSurface(glb->get_render(), sur);
    w = sur->w;
    h = sur->h;
    this->cached_glyphs.insert ({key, TTFCachedGlyphInfo {tex, w, h}});

    SDL_FreeSurface (sur);
  }

  if (tex != nullptr) {
    SDL_Rect dst;
    dst.x = position.x;
    dst.y = position.y; 
    dst.w = w;
    dst.h = h;
    SDL_RenderCopyEx (glb->get_render(), tex, nullptr, &dst, 0, nullptr, SDL_FLIP_NONE);
  }

  return;
}
