#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier& ImageModifier::rotate90 () {
  SDL_Surface* sur = SDL_CreateRGBSurface(
    0, this->texture->h, this->texture->w, 32, 
    0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
  );

  Uint32* arr = (Uint32*)this->texture->pixels;
  Uint32* fin = (Uint32*)sur->pixels;
  uint32_t pos = 0;
  for (int i = 0; i < this->texture->h; i++) {
    for (int j = 0; j < this->texture->w; j++) {
      fin[(j + 1) * this->texture->h - (i + 1)] = arr[pos++];
    }
  }

  SDL_FreeSurface (this->texture);
  this->texture = sur;
  return *this;
}
