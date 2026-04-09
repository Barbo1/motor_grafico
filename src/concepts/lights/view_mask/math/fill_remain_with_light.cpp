#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_surface.h>

void fill_remain_with_lights (SDL_Surface*& img, const Light& light) {
  Uint32* buffer = (Uint32*)img->pixels;
  float add_coef = light.attenuation / std::sqrt(light.intensity);
  Dir2 aux = light.position * add_coef;

#if defined(__AVX2__) && defined(__AVX__)
  __m256 
    mm_color = _mm256_set_ps (
      light.color.r, light.color.g, light.color.b, 0.f, 
      light.color.r, light.color.g, light.color.b, 0.f
    ),
    inv_intensity = _mm256_set_ps (
      0.f, 1.f / light.intensity, 0.f, 1.f / light.intensity, 
      0.f, 1.f / light.intensity, 0.f, 1.f / light.intensity
    ),
    add_coef_x = _mm256_set_ps (
      0.f, 4.f * add_coef, 0.f, 4.f * add_coef,
      0.f, 4.f * add_coef, 0.f, 4.f * add_coef
    ),
    add_coef_y = _mm256_set_ps (
      add_coef, 0.f, add_coef, 0.f,
      add_coef, 0.f, add_coef, 0.f
    ),
    part_coefs = _mm256_set_ps (0.f, 0.f, 0.f, 255.f, 0.f, 0.f, 0.f, 255.f),
    init_pos = _mm256_sub_ps(
      _mm256_castpd_ps(_mm256_broadcastsd_pd(_mm_castps_pd(aux.v))), 
      _mm256_set_ps(0.f, 3.f * add_coef, 0.f, 2.f * add_coef, 0.f, add_coef, 0.f, 0.f)),
    pos;
  __m256i 
    perm1 = _mm256_set_epi32(2,2,2,2,0,0,0,0),
    perm2 = _mm256_set_epi32(6,6,6,6,4,4,4,4),
    mm_opr_mask = _mm256_set_epi8 (
      12,8,4,0,12,8,4,0,12,8,4,0,12,8,4,0,
      12,8,4,0,12,8,4,0,12,8,4,0,12,8,4,0
    );

  pos = init_pos;
  for (uint32_t i = 0; i < (uint32_t)img->h; i++) {
    for (uint32_t j = 0; j < (uint32_t)img->w / 4; j++) {
      __m256 opr = _mm256_fmadd_ps (pos, pos, inv_intensity);
      opr = _mm256_rcp_ps (
        _mm256_add_ps (opr, _mm256_shuffle_ps (opr, opr, 0b01110001))
      );
      __m256 opr1 = _mm256_permutevar8x32_ps(opr, perm1);
      __m256 opr2 = _mm256_permutevar8x32_ps(opr, perm2);

      __m256i part1 = _mm256_shuffle_epi8 (
        _mm256_cvtps_epi32 (
          _mm256_blend_ps (
            _mm256_mul_ps (mm_color, opr1), 
            _mm256_sub_ps (part_coefs, opr1),
            0b00010001
          )
        ), 
        mm_opr_mask
      );
      __m256i part2 = _mm256_shuffle_epi8 (
        _mm256_cvtps_epi32 (
          _mm256_blend_ps (
            _mm256_mul_ps (mm_color, opr2), 
            _mm256_sub_ps (part_coefs, opr2),
            0b00010001
          )
        ), 
        mm_opr_mask
      );
      __m128i whole = _mm256_castsi256_si128(_mm256_unpacklo_epi64(part1, part2));

      __m128i loaded = _mm_loadu_si128((__m128i*)buffer);
      __m128i cond = _mm_cmpeq_epi32(loaded, _mm_setzero_si128());
      __m128i resul = _mm_or_si128(
        _mm_and_si128(cond, whole),
        _mm_andnot_si128(cond, loaded)
      );
      _mm_storeu_si128((__m128i*)buffer, resul);

      buffer += 4;
      pos = _mm256_sub_ps(pos, add_coef_x);
    }
    init_pos = _mm256_sub_ps(init_pos, add_coef_y);
    pos = init_pos;
  }
#else
  Dir2 
    init_pos = aux, 
    add_coef_x = Dir2 {_mm_set_ps (0.f, 0.f, 0.f, add_coef)},
    add_coef_y = Dir2 {_mm_set_ps (0.f, 0.f, add_coef, 0.f)},
    pos;
  __m128 
    mm_color = _mm_set_ps (light.color.r, light.color.g, light.color.b, 0.f),
    inv_intensity = _mm_set_ps (0.f, 0.f, 0.f, 1.f / light.intensity);
  __m128i mm_opr_mask = _mm_set_epi8 (0,0,0,0,0,0,0,0,0,0,0,0,12,8,4,0);

  pos = init_pos;
  for (uint32_t i = 0; i < (uint32_t)img->h; i++) {
    for (uint32_t j = 0; j < (uint32_t)img->w; j++) {
      if (*buffer == 0) {
        __m128 opr = _mm_fmadd_ps (pos.v, pos.v, inv_intensity);
        opr = _mm_rcp_ss (
          _mm_add_ss (
            opr, 
            _mm_shuffle_ps (opr, opr, 0b01010101)
          )
        );
        opr = _mm_shuffle_ps (opr, opr, 0b00000000);
        _mm_storeu_si32 (buffer, _mm_shuffle_epi8 (
          _mm_cvtps_epi32 (
            _mm_move_ss(
              _mm_mul_ps (mm_color, opr), 
              _mm_sub_ss (_mm_set_ps (0.f, 0.f, 0.f, 255.f), opr) 
            )
          ), 
          mm_opr_mask
        ));
      }
      buffer++;
      pos -= add_coef_x;
    }
    init_pos -= add_coef_y;
    pos = init_pos;
  }
#endif
}
