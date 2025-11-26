#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier::ImageModifier (int height, int width, Uint32* pixels) noexcept {
  this->texture = SDL_CreateRGBSurfaceFrom (
    pixels, width, height, 32, width * 4, 
    0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
  );
}
