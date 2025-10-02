#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier::ImageModifier (const ImageModifier & texture) {
  SDL_PixelFormat * fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  this->texture = SDL_ConvertSurface(texture.texture, fmt, 0);
  SDL_FreeFormat(fmt);
}
