#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier& ImageModifier::operator* (const ImageModifier& img) {
  Uint32* arr_t = (Uint32*)this->texture->pixels;
  Uint32* arr_i = (Uint32*)img.texture->pixels;

  SDL_Color color;
  Uint8 a2;
  for (int i = 0; i < std::min(this->texture->h, img.texture->h); i++) {
    Uint32* res = arr_t + i * this->texture->w;
    for (int j = 0; j < std::min(this->texture->w, img.texture->w); j++) {
      SDL_GetRGBA (*res, this->texture->format, &color.r, &color.g, &color.b, &a2);
      SDL_GetRGBA (arr_i[i * img.texture->w + j], this->texture->format, &color.r, &color.g, &color.b, &color.a);
      if (color.a != 0)
        *res = SDL_MapRGBA (this->texture->format, color.r, color.g, color.b, a2);
      res++;
    }
  }

  return *this;
}
