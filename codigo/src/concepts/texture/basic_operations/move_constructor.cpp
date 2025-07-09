#include "../../../../headers/concepts/texture.hpp"

#include <utility>

Texture::Texture (Texture && texture) {
  this->texture = std::exchange(texture.texture, nullptr);
  this->height = std::exchange(texture.height, 0);
  this->width = std::exchange(texture.width, 0);
  this->use_count = std::exchange(texture.use_count, new int(0));
}
