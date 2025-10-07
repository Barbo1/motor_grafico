#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <algorithm>
#include <cstdlib>
#include <cstdint>

ImageModifier& ImageModifier::operator& (const ImageModifier& img) {
  SDL_Surface* sur = SDL_CreateRGBSurface(
    0, this->texture->w, this->texture->h, 
    32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
  );

  if (sur != nullptr) {
    Uint32* fin = (Uint32*)sur->pixels;
    Uint32* arr = (Uint32*)img.texture->pixels;

    uint32_t h = std::min(img.texture->h, this->texture->h);
    uint32_t w = std::min(img.texture->w, this->texture->w);

    uint32_t i = 0;
    Uint32* res = fin;
    for (; i < h; i++) {
      for (uint32_t j = 0; j < w; j++)
        *(res++) = arr[i * img.texture->w + j];
    }

    arr = (Uint32*)this->texture->pixels;

    float coef_1, coef_2;
    SDL_Color color;
    Uint8 a1, a2;
    res = fin;
    for (uint32_t i = 0; i < this->texture->h; i++) {
      for (uint32_t j = 0; j < this->texture->w; j++) {
        SDL_GetRGBA (*res, this->texture->format, &color.r, &color.g, &color.b, &a1);
        SDL_GetRGBA (arr[i * this->texture->w + j], this->texture->format, &color.r, &color.g, &color.b, &a2);
        if (a1 == 0 || a2 == 0)
          *res = 0;
        res++;
      }
    }

    SDL_FreeSurface(this->texture);
    this->texture = sur;
  }
  return *this;
}
