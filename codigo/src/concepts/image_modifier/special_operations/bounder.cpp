#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier ImageModifier::bounder (int* bounds, int height, int width, SDL_Color color) {
  Uint32* pixels = new Uint32[height * width];
  int * piter = bounds;

  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  Uint32 col = SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
  Uint32 non = SDL_MapRGBA(format, 0, 0, 0, 0);
  SDL_FreeFormat(format);

  Uint32 current = non;
  bool inside = false;
  for (int i = 0; i < height * width; i++) {
    while (i == *piter) {
      piter++;
      inside = !inside;
      if (inside) current = col;
      else current = non;
    }

    pixels[i] = current;
  }

  return ImageModifier(height, width, pixels);
}
