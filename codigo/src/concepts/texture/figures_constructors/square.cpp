#include "../../../../headers/concepts/texture.hpp"

Texture Texture::square (SDL_Renderer* render, int height, int base, SDL_Color color) {
  Uint32* pixels = new Uint32[height * base];
  
  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  Uint32 col = SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
  SDL_FreeFormat(format);

  for (int pos = 0; pos < height * base; pos++) {
    pixels[pos] = col;
  }
  
  SDL_Point center;
  center.y = height / 2;
  center.x = base/ 2;
    
  Texture ret = Texture(render, height, base, center, pixels);
  delete [] pixels;
  return ret;
}
