#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <cstdlib>

ViewMask& ViewMask::draw_color_view_mask (const Dir2& position, const std::vector<MaskObject>& segments, const Uint32 color) {
  Uint32* buffer = (Uint32*)img->pixels;
  for (uint32_t i = 0; i < (uint32_t)(img->w * img->h); i++)
    buffer[i] = 0;

  fill_view_with_shadows (this->img, position, segments, color);

  return *this;
}
