#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_surface.h>

void fill_remain_with_lights (SDL_Surface*& img, const Light& light) {
  Uint32* buffer = (Uint32*)img->pixels;

  float add_coef = light.attenuation / std::sqrt(light.intensity);
  Dir2 
    init_pos = -light.position * add_coef, 
    add_coef_x = Dir2 {_mm_set_ps (0.f, 0.f, 0.f, add_coef)},
    add_coef_y = Dir2 {_mm_set_ps (0.f, 0.f, add_coef, 0.f)},
    pos;
  __m128 
    mm_color = _mm_set_ps (light.color.r, light.color.g, light.color.b, 0.f),
    mm_alpha = _mm_set_ps (0.f, 0.f, 0.f, 255.f), 
    inv_intensity = _mm_set_ps (0.f, 0.f, 0.f, 1.f / light.intensity);
  __m128i mm_opr_mask = _mm_set_epi8 (0,0,0,0,0,0,0,0,0,0,0,0,12,8,4,0);

  pos = init_pos;
  for (uint32_t i = 0; i < (uint32_t)img->h; i++) {
    for (uint32_t j = 0; j < (uint32_t)img->w; j++) {
      if (*buffer == 0) {
        __m128 opr = _mm_fmadd_ps (pos.v, pos.v, inv_intensity);
        opr = _mm_rcp_ps (_mm_hadd_ps (opr, opr));
        opr = _mm_shuffle_ps (opr, opr, 0b00000000);
        _mm_storeu_si32 (buffer, _mm_shuffle_epi8 (
          _mm_cvtps_epi32 (_mm_blend_ps (
            _mm_mul_ps (mm_color, opr), 
            _mm_sub_ps (mm_alpha, opr), 
            0b0001
          )), 
          mm_opr_mask
        ));
      }
      buffer++;
      pos += add_coef_x;
    }
    init_pos += add_coef_y;
    pos = init_pos;
  }
}
