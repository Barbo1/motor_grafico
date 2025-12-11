#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_surface.h>

ViewMask::ViewMask (SDL_Surface* img) {
  this->img = img;
}
