#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstdint>

ImageModifier& ImageModifier::operator| (const ImageModifier& img) {
  SDL_Surface* sur = SDL_CreateRGBSurface(
    0, std::max (img.texture->w, this->texture->w), std::max (img.texture->h, this->texture->h), 
    32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
  );

  if (sur != nullptr) {
    uint32_t h = this->texture->h;
    uint32_t w = this->texture->w;
    Uint32* arr = (Uint32*)this->texture->pixels;
    Uint32* res = (Uint32*)sur->pixels;
    for (uint32_t i = 0; i < h; i++) {
      memcpy (res, arr, sizeof(Uint32) * w);
      arr += w;
      res += sur->w;
    }

    arr = (Uint32*)img.texture->pixels;

    for (uint32_t i = 0; i < static_cast<uint32_t>(img.texture->h); i++) {
      Uint32* res = (Uint32*)sur->pixels + i * sur->w;
      for (uint32_t j = 0; j < static_cast<uint32_t>(img.texture->w); j++) {
        SDL_Color color1, color2;
        SDL_GetRGBA (*res, this->texture->format, &color1.r, &color1.g, &color1.b, &color1.a);
        SDL_GetRGBA (*arr, this->texture->format, &color2.r, &color2.g, &color2.b, &color2.a);
        float alpha1 = color1.a;
        float alpha2 = color2.a;
        float alpham = std::max (alpha1, alpha2);
        float coef_1, coef_2;
        if (alpha2 == alpham) {
          coef_1 = alpha1 / alpham;
          coef_2 = 1.f - coef_1;
        } else {
          coef_2 = alpha2 / alpham;
          coef_1 = 1.f - coef_2;
        }

        *(res++) =  
          (std::min ((uint32_t)255, static_cast<uint32_t>(coef_1*color1.r + coef_2*color2.r)) << 24) |
          (std::min ((uint32_t)255, static_cast<uint32_t>(coef_1*color1.g + coef_2*color2.g)) << 16) |
          (std::min ((uint32_t)255, static_cast<uint32_t>(coef_1*color1.b + coef_2*color2.b)) << 8) |
          static_cast<uint32_t>(alpham);
        arr++;
      }
    }

    if (this->aquired)
      std::free(this->texture->pixels);
    SDL_FreeSurface(this->texture);

    this->texture = sur;
    this->aquired = 0;
  }
  return *this;
}
