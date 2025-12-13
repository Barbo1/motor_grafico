#include "../../../../headers/concepts/image_modifier.hpp"
#include <utility>

ImageModifier::ImageModifier (ImageModifier && texture) noexcept {
  this->texture = std::exchange (texture.texture, nullptr);
  this->aquired = std::exchange (texture.aquired, 0);
}
