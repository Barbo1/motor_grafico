#include "../../../../../headers/concepts/lights.hpp"
#include <cstdint>
#include <immintrin.h>
#include <smmintrin.h>

ViewMask& ViewMask::draw_light_uniform_mask (const Light& light) {
  Uint32* buffer = (Uint32*)img->pixels;
  for (uint32_t i = 0; i < (uint32_t)(img->w * img->h); i++)
    buffer[i] = 0;

  fill_remain_with_lights(this->img, light);

  return *this;
}
