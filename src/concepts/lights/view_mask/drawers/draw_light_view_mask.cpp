#include "../../../../../headers/concepts/lights.hpp"

ViewMask& ViewMask::draw_light_view_mask (const Light& light, const std::vector<MaskObject>& segments, const Uint32 shadow_color) {
  Uint32* buffer = (Uint32*)img->pixels;
  for (uint32_t i = 0; i < (uint32_t)(img->w * img->h); i++)
    buffer[i] = 0;

  fill_view_with_shadows (this->img, light.position, segments, shadow_color);
  fill_view_with_lights (this->img, light);

  return *this;
}
