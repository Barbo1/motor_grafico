#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier ImageModifier::square (int height, int base, SDL_Color color) {

  Uint32* pixels = new Uint32[height * base];
  Uint32 col = (color.r << 24) | (color.g << 16) | (color.b << 8) | color.a;
  for (int pos = 0; pos < height * base; pos++)
    pixels[pos] = col;
  
  ImageModifier ret = ImageModifier(height, base, pixels);

  return ret;
}
