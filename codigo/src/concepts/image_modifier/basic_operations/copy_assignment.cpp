#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_surface.h>

ImageModifier& ImageModifier::operator= (const ImageModifier & texture) {
  if (this->texture != nullptr)
    SDL_FreeSurface(this->texture);

  SDL_PixelFormat * fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  this->texture = SDL_ConvertSurface(texture.texture, fmt, 0);
  SDL_FreeFormat(fmt);

  return *this;
}
