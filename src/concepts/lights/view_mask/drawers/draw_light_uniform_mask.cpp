#include "../../../../../headers/concepts/lights.hpp"
#include <cstdint>
#include <immintrin.h>
#include <smmintrin.h>

ViewMask& ViewMask::draw_light_uniform_mask (const Light& light) {
  uint64_t* buffer = (uint64_t*)img->pixels;

  float add_coef = light.attenuation / std::sqrt(light.intensity);
  Dir2 aux = light.position * add_coef;

  __m256
    mm_color = _mm256_set_ps (
      light.color.r, light.color.g, light.color.b, 0.f,
      light.color.r, light.color.g, light.color.b, 0.f
    ),
    mm_alpha = _mm256_set_ps (0.f, 0.f, 0.f, 255.f, 0.f, 0.f, 0.f, 255.f); 

  __m128 
    inv_intensity = _mm_set_ps (0.f, 1.f / light.intensity, 0.f, 1.f / light.intensity),
    add_coef_x = _mm_set_ps (0.f, 2.f*add_coef, 0.f, 2.f*add_coef),
    add_coef_y = _mm_set_ps (add_coef, 0.f, add_coef, 0.f),
    init_pos = _mm_set_ps (aux.y, aux.x, aux.y - add_coef, aux.x - add_coef),
    pos;

  __m256i shuffle_mask = _mm256_setr_epi8 (
    0, 4, 8, 12, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 8, 12, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
  );

  pos = init_pos;
  for (uint32_t i = 0; i < (uint32_t)img->h; i++) {
    for (uint32_t j = 0; j < (uint32_t)img->w / 2; j++) {
      __m128 opr = _mm_fmadd_ps (pos, pos, inv_intensity);
      opr = _mm_rcp_ps (_mm_add_ps (opr, _mm_shuffle_ps (opr, opr, 0b10110001)));

      __m256 opr2 = _mm256_set_m128 (
        _mm_broadcastss_ps (_mm_shuffle_ps (opr, opr, 0b00000010)), 
        _mm_broadcastss_ps (opr)
      );

      __m256i opr3 = _mm256_shuffle_epi8 (
        _mm256_cvtps_epi32 (_mm256_blend_ps (
          _mm256_mul_ps (mm_color, opr2), 
          _mm256_sub_ps (mm_alpha, opr2), 
          0b00010001
        )), 
        shuffle_mask
      );

      _mm_storeu_si64 (buffer, _mm_unpacklo_epi32 ( 
        _mm256_extractf128_si256 (opr3, 1),
        _mm256_castsi256_si128 (opr3)
      ));

      buffer++;
      pos = _mm_sub_ps (pos, add_coef_x);
    }
    pos = init_pos = _mm_sub_ps (init_pos, add_coef_y);
  }

  return *this;
}
