#include "../../../../headers/concepts/image_modifier.hpp"
#include <cstdint>

ImageModifier& ImageModifier::operator& (const ImageModifier& img) {
  SDL_Surface* sur = SDL_CreateRGBSurface(
    0, this->texture->w, this->texture->h, 
    32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
  );

  if (sur != nullptr) {
    Uint32* fin = (Uint32*)sur->pixels;
    Uint32* arr = (Uint32*)img.texture->pixels;

    uint32_t i = 0;
    uint32_t calc_w = std::min(img.texture->w, this->texture->w);
    uint32_t calc_h = std::min(img.texture->h, this->texture->h);
    for (; i < calc_h; i++)
      memcpy (fin + i * this->texture->w, arr + i * img.texture->w, calc_w * sizeof(Uint32));

    arr = (Uint32*)this->texture->pixels;

    for (int i = 0; i < this->texture->h; i++)
      for (int j = 0; j < this->texture->w; j++, fin++, arr++)
        if ((*fin & 0x000000FF) == 0 || (*arr & 0x000000FF) == 0)
          *fin = 0;

    if (this->aquired)
      std::free(this->texture->pixels);
    SDL_FreeSurface(this->texture);

    this->texture = sur;
    this->aquired = 0;
  }
  return *this;
}
