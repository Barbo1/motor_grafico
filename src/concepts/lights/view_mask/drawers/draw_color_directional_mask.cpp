#include "../../../../../headers/concepts/lights.hpp"

ViewMask& ViewMask::draw_color_directional_mask (
  const Dir2& direction, const std::vector<MaskObject>& segments, const Uint32 shadow_color
) {
  Uint32* buffer = (Uint32*)img->pixels;
  for (uint32_t i = 0; i < (uint32_t)(img->w * img->h); i++)
    buffer[i] = 0;

  fill_directional_with_shadows (this->img, direction, segments, shadow_color);

  return *this;
}
