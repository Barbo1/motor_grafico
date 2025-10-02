#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier ImageModifier::square (int height, int base, SDL_Color color) {
  Uint32* pixels = new Uint32[height * base];
  
  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  Uint32 col = SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
  SDL_FreeFormat(format);

  for (int pos = 0; pos < height * base; pos++) {
    pixels[pos] = col;
  }
  
  return ImageModifier(height, base, pixels);
}
