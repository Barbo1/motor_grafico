#include "../../../../headers/concepts/image_modifier.hpp"
#include <cstdint>

ImageModifier& ImageModifier::rotate270 () {
  SDL_Surface* sur = SDL_CreateRGBSurface(
    0, this->texture->h, this->texture->w, 32, 
    0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
  );

  Uint32* arr = (Uint32*)this->texture->pixels;
  Uint32* fin = (Uint32*)sur->pixels;
  uint32_t w = this->texture->w;
  uint32_t h = this->texture->h;
  for (uint32_t i = 0; i < h; i++) {
    for (uint32_t j = 0; j < w; j++) {
      fin[j * h + i] = arr[(i + 1) * w - (j + 1)];
    }
  }

  SDL_FreeSurface (this->texture);
  this->texture = sur;
  return *this;
}
