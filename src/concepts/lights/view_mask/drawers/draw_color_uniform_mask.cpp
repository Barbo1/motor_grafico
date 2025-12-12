#include "../../../../../headers/concepts/lights.hpp"

ViewMask& ViewMask::draw_color_uniform_mask (const Uint32 color) {
  Uint32* buffer = (Uint32*)this->img->pixels;
  for (uint32_t i = 0; i < (uint32_t)(img->w * img->h); i++)
    buffer[i] = color;

  return *this;
}
