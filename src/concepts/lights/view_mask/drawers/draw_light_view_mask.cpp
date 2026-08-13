#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_render.h>
#include <cstdint>

ViewMask& ViewMask::draw_light_view_mask (
  DynamicalArena& darena,
  const MaskObjectList& segments,
  const Light& light, 
  const Dir2& screen_dims 
) {
  Uint32* buffer = (Uint32*)img->pixels;
  for (uint32_t i = 0; i < (uint32_t)(img->w * img->h); i++)
    buffer[i] = 0;

  MaskObjectList filtered = filter_lines_point_view(darena, segments, light, screen_dims);

  fill_view_with_shadows (this->img, darena, filtered, light.position);
  fill_remain_with_lights (this->img, light);

  if (filtered.obj != nullptr)
    darena.complete_free_mo(filtered.obj);

  return *this;
}
