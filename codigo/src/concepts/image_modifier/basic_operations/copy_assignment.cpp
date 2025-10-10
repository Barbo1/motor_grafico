#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_surface.h>

ImageModifier& ImageModifier::operator= (const ImageModifier & texture) {
  if (this->texture != nullptr)
    SDL_FreeSurface(this->texture);
  this->texture = SDL_ConvertSurface(texture.texture, texture.texture->format, 0);
  return *this;
}
