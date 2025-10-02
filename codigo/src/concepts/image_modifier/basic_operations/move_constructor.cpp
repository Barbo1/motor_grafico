#include "../../../../headers/concepts/image_modifier.hpp"
#include <utility>

ImageModifier::ImageModifier (ImageModifier && texture) {
  this->texture = std::exchange (texture.texture, nullptr);
}
