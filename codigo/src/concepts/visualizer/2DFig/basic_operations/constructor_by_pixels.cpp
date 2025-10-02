#include "../../../../../headers/concepts/visualizer.hpp"
#include <SDL2/SDL_render.h>

template<>
Visualizer<D2FIG>::Visualizer (SDL_Renderer* render, int height, int width, Uint32* pixels) {
  this->use_count = new int(1);
  this->width = width;
  this->height = height;
  SDL_Surface* sur = SDL_CreateRGBSurfaceFrom (
    pixels, width, height, 32, width * 4, 
    0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
  );
  this->texture = SDL_CreateTextureFromSurface(render, sur);
  SDL_FreeSurface(sur);
}
