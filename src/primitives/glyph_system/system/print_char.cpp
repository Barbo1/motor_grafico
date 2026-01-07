#include "../../../../headers/primitives/glyph_system.hpp"
#include <cstdint>

void GlyphsSystem::print (char16_t character, uint32_t size, Dir2 position) {
  uint32_t w, h;
  SDL_Texture* tex;
  auto founded = this->cached_glyphs.find(character);
  if (founded != this->cached_glyphs.end()) {
    tex = founded->second.tex;
    w = founded->second.w;
    h = founded->second.h;
  } else {
    SDL_Surface* sur = this->raster (character, size);
    tex = SDL_CreateTextureFromSurface(glb->get_render(), sur);
    w = sur->w;
    h = sur->h;
    SDL_FreeSurface (sur);
  }
  SDL_Rect dst;
  dst.x = position.x;
  dst.y = position.y; 
  dst.w = w;
  dst.h = h;
  SDL_RenderCopyEx (glb->get_render(), tex, nullptr, &dst, 0, nullptr, SDL_FLIP_NONE);
  return;
}
