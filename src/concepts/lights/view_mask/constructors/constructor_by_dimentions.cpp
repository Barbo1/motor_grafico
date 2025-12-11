#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_surface.h>
#include <cstdint>
#include <cstdio>

ViewMask::ViewMask (uint32_t width, uint32_t height) {
  Uint32* buffer = static_cast<Uint32*>(std::aligned_alloc(16, width * height * sizeof (Uint32)));
  this->img = SDL_CreateRGBSurfaceFrom (
    buffer, width, height, 32, width * 4, 
    0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
  );
}
