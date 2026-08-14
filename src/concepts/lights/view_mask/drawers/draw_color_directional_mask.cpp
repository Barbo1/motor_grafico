#include "../../../../../headers/concepts/lights.hpp"

ViewMask& ViewMask::draw_color_directional_mask (
  DynamicalArena& darena, 
  const MaskObjectList& segments,
  const Dir2& direction
) {
  Uint32* buffer = (Uint32*)img->pixels;
  for (uint32_t i = 0; i < (uint32_t)(img->w * img->h); i++)
    buffer[i] = 0;

  fill_directional_with_shadows (buffer, img->w, img->h, darena, segments, direction);

  return *this;
}
