
#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_surface.h>

ViewMask::~ViewMask () {
  std::free(this->img->pixels);
  SDL_FreeSurface(this->img);
}
