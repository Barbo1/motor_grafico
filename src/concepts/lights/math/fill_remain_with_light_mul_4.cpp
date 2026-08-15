#include "../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_surface.h>
#include <emmintrin.h>
#include <immintrin.h>

void fill_remain_with_lights_4 (
  Uint32*& buffer, 
  int32_t width, 
  int32_t height, 
  const Light& light
) {
  float add_coef = light.attenuation / std::sqrt(light.intensity);
  Dir2 aux = light.position * add_coef;
  __m128
    pos_1, pos_2,
    coef_needed = _mm_set_ps(0.f, add_coef, 0.f, 0.f),
    init_pos = _mm_sub_ps(aux.v, coef_needed),
    add_coef_x_mask = _mm_set_ps (0.f, add_coef, 0.f, add_coef),
    add_coef_x_4 = _mm_mul_ps(add_coef_x_mask, _mm_set1_ps(4.f)),
    add_coef_x_2 = _mm_mul_ps(add_coef_x_mask, _mm_set1_ps(4.f)),
    add_coef_y = _mm_set_ps (add_coef, 0.f, add_coef, 0.f),
    mm_color = _mm_set_ps (light.color.r, light.color.g, light.color.b, 0.f),
    inv_intensity = _mm_set_ps (0.f, 1.f / light.intensity, 0.f, 1.f / light.intensity);
  __m128 ext_255 = _mm_set1_ps(255.f);

  pos_1 = init_pos;
  pos_2 = _mm_sub_ps(init_pos, add_coef_x_2);
  for (int32_t i = 0; i < height; i++) {
    for (int32_t j = 0; j < width; j+=4) {
      __m128 opr_1 = _mm_fmadd_ps (pos_1, pos_1, inv_intensity);
      __m128 opr_2 = _mm_fmadd_ps (pos_2, pos_2, inv_intensity);
      __m128 opr = _mm_rcp_ps (_mm_hadd_ps (opr_1, opr_2));

      __m128 coefs_1 = _mm_permute_ps(opr, 0);
      __m128 coefs_2 = _mm_permute_ps(opr, 0b01010101);
      __m128 coefs_3 = _mm_permute_ps(opr, 0b10101010);
      __m128 coefs_4 = _mm_permute_ps(opr, 0b11111111);
      __m128 alphas = _mm_sub_ps (ext_255, opr);

      __m128i result = _mm_packus_epi16(
        _mm_packus_epi32(
          _mm_cvtps_epi32 (_mm_insert_ps (_mm_mul_ps (mm_color, coefs_1), alphas, 0b00000000)),
          _mm_cvtps_epi32 (_mm_insert_ps (_mm_mul_ps (mm_color, coefs_2), alphas, 0b01000000))
        ),
        _mm_packus_epi32(
          _mm_cvtps_epi32 (_mm_insert_ps (_mm_mul_ps (mm_color, coefs_3), alphas, 0b10000000)),
          _mm_cvtps_epi32 (_mm_insert_ps (_mm_mul_ps (mm_color, coefs_4), alphas, 0b11000000))
        )
      );

      __m128i mem_info = _mm_xor_si128(
        _mm_cmpeq_epi32(_mm_loadu_si128((__m128i*)buffer), _mm_setzero_si128()),
        _mm_setzero_si128()
      );
      _mm_maskmoveu_si128(result, mem_info, (char*)buffer);

      buffer+=4;
      pos_1 = _mm_sub_ps(pos_1, add_coef_x_4);
      pos_2 = _mm_sub_ps(pos_2, add_coef_x_4);
    }

    init_pos = _mm_sub_ps(init_pos, add_coef_y);
    pos_1 = init_pos;
    pos_2 = _mm_sub_ps(init_pos, add_coef_x_2);
  }
}
