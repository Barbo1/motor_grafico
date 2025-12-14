#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <immintrin.h>

/* Given the colors c1 = (r1, g1, b1, a1) and c2 = (r2, g2, b2, a2), this functions
 * makes, for every pair of pixels in both sides:
 *  na1 = 255 - a1
 *  na2 = 255 - a2
 *  q = na1 + na2
 *  c = ((r1, g1, b1, 0) * na1/q + (r2, g2, b2, 0) * na2/q) + (0, 0, 0, min (a1, a2))
 *
 *  which is meant to be the color of the resulting light from both sides.
 * */

ViewMask& ViewMask::operator| (const ViewMask& mask) {
  if (this->img->w == mask.img->w && this->img->h == mask.img->h) {
    Uint32* buffer_this = (Uint32*)this->img->pixels;
    Uint32* buffer_mask = (Uint32*)mask.img->pixels;

    uint32_t total = (uint32_t)this->img->h * this->img->w;
    uint32_t total_2 = total / 4;
    
    __m256 coefs_2 = _mm256_set_ps (0.f, 0.f, 0.f, 255.f, 0.f, 0.f, 0.f, 255.f);

    for (uint32_t i = 0; i < total_2; i++) {

      /* first iteration. */
      __m128i charged_t = _mm_stream_load_si128 ((__m128i*)buffer_this);
      __m128i charged_m = _mm_stream_load_si128 ((__m128i*)buffer_mask);

      __m256 oprt = _mm256_cvtepi32_ps (_mm256_cvtepi8_epi32 (charged_t));
      __m256 oprm = _mm256_cvtepi32_ps (_mm256_cvtepi8_epi32 (charged_m));
     
      __m256 nat = _mm256_sub_ps (coefs_2, oprt);
      __m256 nam = _mm256_sub_ps (coefs_2, oprm);
      __m256 q = _mm256_rcp_ps (_mm256_add_ps (nat, nam));
      nat = _mm256_mul_ps (q, nat);
      nam = _mm256_mul_ps (q, nam);

      __m256i res = _mm256_cvtps_epi32 (_mm256_blend_ps (
        _mm256_add_ps (
          _mm256_mul_ps (_mm256_shuffle_ps (nat, nat, 0b00000000), oprt), 
          _mm256_mul_ps (_mm256_shuffle_ps (nam, nam, 0b00000000), oprm)
        ), 
        _mm256_min_ps (oprt, oprm),
        0b00010001
      ));
      
      __m128 res1 = _mm_castsi128_ps(_mm256_castsi256_si128 (
        _mm256_packs_epi16 (
          _mm256_permute4x64_epi64(
            _mm256_packs_epi32 (
              res, 
              _mm256_undefined_si256()
            ), 0b00001000
          ), _mm256_undefined_si256()
        )
      ));

      /* second iteration. */
      charged_t = _mm_shuffle_epi32 (charged_t, 0b00001110);
      charged_m = _mm_shuffle_epi32 (charged_m, 0b00001110);

      oprt = _mm256_cvtepi32_ps (_mm256_cvtepi8_epi32 (charged_t));
      oprm = _mm256_cvtepi32_ps (_mm256_cvtepi8_epi32 (charged_m));
     
      nat = _mm256_sub_ps (coefs_2, oprt);
      nam = _mm256_sub_ps (coefs_2, oprm);
      q = _mm256_rcp_ps (_mm256_add_ps (nat, nam));
      nat = _mm256_mul_ps (q, nat);
      nam = _mm256_mul_ps (q, nam);

      res = _mm256_cvtps_epi32 (_mm256_blend_ps (
        _mm256_add_ps (
          _mm256_mul_ps (_mm256_shuffle_ps (nat, nat, 0b00000000), oprt), 
          _mm256_mul_ps (_mm256_shuffle_ps (nam, nam, 0b00000000), oprm)
        ), 
        _mm256_min_ps (oprt, oprm),
        0b00010001
      ));

      __m128 res2 = _mm_castsi128_ps(_mm256_castsi256_si128 (
        _mm256_packs_epi16 (
          _mm256_permute4x64_epi64(
            _mm256_packs_epi32 (
              res, 
              _mm256_undefined_si256()
            ), 0b00001000
          ), _mm256_undefined_si256()
        )
      ));

      _mm_stream_ps ((float*)buffer_this, _mm_shuffle_ps(res1, res2, 0b01000100));

      buffer_this += 4;
      buffer_mask += 4;
    }
    
    __m128 coefs = _mm_set_ps (0.f, 0.f, 0.f, 255.f);
    __m128i mm_opr_mask = _mm_set_epi8 (0,0,0,0,0,0,0,0,0,0,0,0,12,8,4,0);

    for (uint32_t i = 0; i < total_2 % 4; i++) {
      __m128 oprt = _mm_cvtepi32_ps (_mm_cvtepi8_epi32 (_mm_loadu_si32 (buffer_this)));
      __m128 oprm = _mm_cvtepi32_ps (_mm_cvtepi8_epi32 (_mm_loadu_si32 (buffer_mask)));
     
      __m128 qt = _mm_sub_ps (coefs, oprt);
      __m128 qm = _mm_sub_ps (coefs, oprm);
      __m128 q = _mm_rcp_ps (_mm_add_ps (qt, qm));
      qt = _mm_mul_ps (q, qt);
      qm = _mm_mul_ps (q, qm);

      _mm_storeu_si32 (buffer_this, _mm_shuffle_epi8 (
        _mm_cvtps_epi32 (_mm_blend_ps (
          _mm_add_ps (
            _mm_mul_ps (
              _mm_shuffle_ps (qt, qt, 0b00000000), oprt
            ), _mm_mul_ps (
              _mm_shuffle_ps (qm, qm, 0b00000000), oprm
            )
          ), 
          _mm_min_ps (oprt, oprm), 
          0b0001
        )), 
        mm_opr_mask
      ));

      buffer_this++;
      buffer_mask++;
    }
  }

  return *this;
}
