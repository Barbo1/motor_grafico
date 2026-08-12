#include "../../../../../headers/concepts/lights.hpp"
#include <cstdint>

ViewMask& ViewMask::draw_light_view_mask (
  const Light& light, 
  DynamicalArena& darena,
  const SDL_Rect& screem_metrics, 
  MaskObjectList segments
) {
  Uint32* buffer = (Uint32*)img->pixels;
  for (uint32_t i = 0; i < (uint32_t)(img->w * img->h); i++)
    buffer[i] = 0;

  Dir2 screen_dims = Dir2(screem_metrics.w, screem_metrics.h);
  Dir2 screen_pos = Dir2(screem_metrics.x, screem_metrics.y);

  MaskObjectList filtered = filter_lines_point_view(darena, segments, light, screen_pos, screen_dims);

  fill_view_with_shadows (this->img, darena, filtered, light.position);
  fill_remain_with_lights (this->img, light);

  if (filtered.obj != nullptr)
    darena.complete_free_mo(filtered.obj);

  return *this;
}
