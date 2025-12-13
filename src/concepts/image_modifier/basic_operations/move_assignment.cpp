#include "../../../../headers/concepts/image_modifier.hpp"
#include <utility>

ImageModifier& ImageModifier::operator= (ImageModifier && texture) noexcept {
  if (this->texture != nullptr) {
    if (this->aquired)
      std::free (this->texture->pixels);
    SDL_FreeSurface (this->texture);
  }

  this->texture = std::exchange (texture.texture, nullptr);
  this->aquired = std::exchange (texture.aquired, 0);
  return *this;
}
