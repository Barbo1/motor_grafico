#include "../../../../headers/concepts/image_modifier.hpp"
#include <SDL2/SDL_surface.h>

ImageModifier& ImageModifier::operator= (const ImageModifier & texture) noexcept {
  if (this->texture != nullptr) {
    if (this->aquired)
      std::free (this->texture->pixels);
    SDL_FreeSurface(this->texture);
  }
  this->texture = SDL_ConvertSurface(texture.texture, texture.texture->format, 0);
  return *this;
}
