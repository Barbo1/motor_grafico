#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier::ImageModifier (const ImageModifier & texture) {
  this->texture = SDL_ConvertSurface(texture.texture, texture.texture->format, 0);
}
