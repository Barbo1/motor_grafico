#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstdint>

SDL_Color ImageModifier::operator() (uint32_t i, uint32_t j) {
  if (i < static_cast<uint32_t>(this->texture->h) && 
    j < static_cast<uint32_t>(this->texture->w) && 
    this->texture->pixels != nullptr
  ) {
    SDL_Color color;
    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    SDL_GetRGBA(
      ((uint32_t*)this->texture->pixels)[i * this->texture->w + j],
      format, &color.r, &color.g, &color.b, &color.a
    );
    return color;
  }
  return SDL_Color {0, 0, 0, 0};
}
