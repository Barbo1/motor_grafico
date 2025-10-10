#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier& ImageModifier::operator| (const ImageModifier& img) {
  SDL_Surface* sur = SDL_CreateRGBSurface(
    0, std::max (img.texture->w, this->texture->w), std::max (img.texture->h, this->texture->h), 
    32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
  );

  if (sur != nullptr) {
    uint32_t h = this->texture->h;
    uint32_t w = this->texture->w;

    Uint32* fin = (Uint32*)sur->pixels;
    Uint32* arr = (Uint32*)this->texture->pixels;

    uint32_t i = 0;
    Uint32* res = fin;
    for (; i < h; i++) {
      uint32_t j = 0;
      for (; j < w; j++)
        *(res++) = arr[i * w + j];
      for (; j < sur->w; j++)
        *(res++) = 0;
    }
    for (; i < sur->h; i++) {
      for (uint32_t j = 0; j < sur->w; j++)
        *(res++) = 0;
    }

    arr = (Uint32*)img.texture->pixels;
    w = img.texture->w;

    float coef_1, coef_2;
    SDL_Color color1, color2;
    for (uint32_t i = 0; i < img.texture->h; i++) {
      Uint32* res = fin + (i * sur->w);
      for (uint32_t j = 0; j < w; j++) {
        SDL_GetRGBA (*res, this->texture->format, &color1.r, &color1.g, &color1.b, &color1.a);
        SDL_GetRGBA (arr[i * w + j], this->texture->format, &color2.r, &color2.g, &color2.b, &color2.a);
        uint32_t alpha = std::max (color1.a, color2.a);
        if (color2.a == alpha) {
          coef_1 = (float)color1.a / alpha;
          coef_2 = 1 - coef_1;
        } else {
          coef_2 = (float)color2.a / alpha;
          coef_1 = 1 - coef_2;
        }

        *(res++) = SDL_MapRGBA (this->texture->format, 
          std::min ((uint32_t)255, static_cast<uint32_t>(coef_1*color1.r + coef_2*color2.r)),
          std::min ((uint32_t)255, static_cast<uint32_t>(coef_1*color1.g + coef_2*color2.g)),
          std::min ((uint32_t)255, static_cast<uint32_t>(coef_1*color1.b + coef_2*color2.b)),
          alpha
        );
      }
    }

    SDL_FreeSurface(this->texture);
    this->texture = sur;
  }
  return *this;
}
