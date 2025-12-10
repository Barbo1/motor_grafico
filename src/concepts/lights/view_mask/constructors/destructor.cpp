
#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_surface.h>

ViewMask::~ViewMask () {
  void* ptr = this->img->pixels;
  SDL_FreeSurface(this->img);
  std::free(ptr);
}
