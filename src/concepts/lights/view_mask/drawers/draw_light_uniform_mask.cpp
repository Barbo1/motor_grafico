#include "../../../../../headers/concepts/lights.hpp"
#include <cstdint>
#include <immintrin.h>
#include <smmintrin.h>

ViewMask& ViewMask::draw_light_uniform_mask (const Light& light) {
  Uint32* buffer = (Uint32*)img->pixels;
  float add_coef = light.attenuation / std::sqrt(light.intensity);
  Dir2 aux = light.position * add_coef;

  __m128
    pos,
    init_pos = aux.v,
    add_coef_x = _mm_set_ps (0.f, add_coef, 0.f, add_coef),
    add_coef_y = _mm_set_ps (add_coef, 0.f, add_coef, 0.f),
    mm_color = _mm_set_ps (light.color.r, light.color.g, light.color.b, 0.f),
    inv_intensity = _mm_set_ps (0.f, 1.f / light.intensity, 0.f, 1.f / light.intensity);
  __m128i mm_opr_mask = _mm_set_epi8 (0,0,0,0,0,0,0,0,0,0,0,0,12,8,4,0);

  pos = init_pos;
  for (uint32_t i = 0; i < (uint32_t)img->h; i++) {
    for (uint32_t j = 0; j < (uint32_t)img->w; j++) {
      __m128 opr = _mm_fmadd_ps (pos, pos, inv_intensity);
      opr = _mm_rcp_ps (_mm_add_ps (opr, _mm_permute_ps (opr, 0b00010001)));
      _mm_storeu_si32 (buffer, _mm_shuffle_epi8 (
        _mm_cvtps_epi32 (
          _mm_move_ss (
            _mm_mul_ps (mm_color, opr), 
            _mm_sub_ss (_mm_set_ss (255.f), opr) 
          )
        ), 
        mm_opr_mask
      ));
      buffer++;
      pos -= add_coef_x;
    }
    init_pos -= add_coef_y;
    pos = init_pos;
  }

  return *this;
}
