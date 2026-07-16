#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier ImageModifier::bound_constructor (int* bounds, int height, int width, SDL_Color color) {
  Uint32* pixels = new Uint32[height * width];
  int * piter = bounds;
  Uint32 col = (color.r << 24) | (color.g << 16) | (color.b << 8) | color.a;
  Uint32 current = 0;
  for (int i = 0; i < height * width; i++) {
    while (i == *piter) {
      piter++;
      current ^= col;
    }
    pixels[i] = current;
  }

  return ImageModifier(height, width, pixels);
}
