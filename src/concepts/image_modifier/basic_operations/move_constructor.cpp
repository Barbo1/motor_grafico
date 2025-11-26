#include "../../../../headers/concepts/image_modifier.hpp"
#include <utility>

ImageModifier::ImageModifier (ImageModifier && texture) noexcept {
  this->texture = std::exchange (texture.texture, nullptr);
}
