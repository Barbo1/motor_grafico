#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier ImageModifier::bound_constructor (int* bounds, Uint32* texture_buffer, int height, int width, SDL_Color color) {
  int * piter = bounds;

  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  Uint32 col = SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
  SDL_FreeFormat(format);

  Uint32 current = 0;
  for (int i = 0; i < height * width; i++) {
    while (i == *piter) {
      piter++;
      current ^= col;
    }
    texture_buffer[i] = current;
  }

  return ImageModifier(height, width, texture_buffer);
}
