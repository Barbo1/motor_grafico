#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier& ImageModifier::xshear (float angle) {
  float m = 1 / std::tan (angle);

  SDL_Surface* sur = SDL_CreateRGBSurface(
    0, this->texture->w + this->texture->h * m, this->texture->h, 32, 
    0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
  );

  Uint32* fin = (Uint32*)sur->pixels;
  Uint32* arr = (Uint32*)this->texture->pixels;
  uint32_t i = 0, rowf = 0, rowa = 0, j = 0;
  for (; i < static_cast<uint32_t>(this->texture->h); i++, rowf += sur->w, rowa += this->texture->w) {
    for (; j < rowf + i * m; j++)
      fin[j] = 0;
    for (uint32_t k = rowa; k < rowa + this->texture->w; k++, j++)
      fin[j] = arr[k];
    for (; j < sur->w + rowf; j++)
      fin[j] = 0;
  }

  SDL_FreeSurface (this->texture);
  this->texture = sur;
  return *this;
}
