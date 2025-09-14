#include "../../../../../headers/concepts/visualizer.hpp"

template <VisualType T>
Visualizer<T> Visualizer<T>::square (SDL_Renderer* render, int height, int base, SDL_Color color) {
  Uint32* pixels = new Uint32[height * base];
  
  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  Uint32 col = SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
  SDL_FreeFormat(format);

  for (int pos = 0; pos < height * base; pos++) {
    pixels[pos] = col;
  }
  
  Visualizer<T> ret = Visualizer<T>(render, height, base, pixels);
  delete [] pixels;
  return ret;
}

template Visualizer<D2FIG> Visualizer<D2FIG>::square (
  SDL_Renderer* render, 
  int height, 
  int base, 
  SDL_Color color
);
