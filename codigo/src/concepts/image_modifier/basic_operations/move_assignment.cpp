#include "../../../../headers/concepts/image_modifier.hpp"
#include <utility>

ImageModifier& ImageModifier::operator= (ImageModifier && texture) {
  if (this->texture != nullptr)
    SDL_FreeSurface(this->texture);

  this->texture = std::exchange (texture.texture, nullptr);
  return *this;
}
