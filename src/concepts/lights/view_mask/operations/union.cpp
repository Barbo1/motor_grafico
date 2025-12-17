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
    __m128i* buffer_this = (__m128i*)this->img->pixels;
    __m128i* buffer_mask = (__m128i*)mask.img->pixels;

    __m256 coefs_2 = _mm256_set_ps (0.f, 0.f, 0.f, 255.f, 0.f, 0.f, 0.f, 255.f);
    __m128i mm_opr_mask = _mm_set_epi8 (0,0,0,0,0,0,0,0,0,0,0,0,12,8,4,0);

    for (uint32_t i = 0; i < (uint32_t)this->img->h * this->img->w / 4; i++) {

      /* first iteration. */
      __m128i charged_t = _mm_stream_load_si128 ((__m128i*)buffer_this);
      __m128i charged_m = _mm_stream_load_si128 ((__m128i*)buffer_mask);

      __m256 oprt = _mm256_cvtepi32_ps (_mm256_cvtepu8_epi32 (charged_t));
      __m256 oprm = _mm256_cvtepi32_ps (_mm256_cvtepu8_epi32 (charged_m));
     
      __m256 nat = _mm256_sub_ps (coefs_2, oprt);
      __m256 nam = _mm256_sub_ps (coefs_2, oprm);
      __m256 q = _mm256_rcp_ps (_mm256_add_ps (nat, nam));
      nat = _mm256_mul_ps (q, nat);
      nam = _mm256_mul_ps (q, nam);

      __m256i res = _mm256_cvtps_epi32 (_mm256_fmadd_ps (
        _mm256_shuffle_ps (nat, nat, 0b00000000), oprt, 
        _mm256_mul_ps (_mm256_shuffle_ps (nam, nam, 0b00000000), oprm)
      ));
      
      __m128i res1 = _mm_unpacklo_epi32 (
        _mm_shuffle_epi8 (_mm256_castsi256_si128 (res), mm_opr_mask),
        _mm_shuffle_epi8 (_mm256_extractf128_si256 (res, 1), mm_opr_mask)
      );

      /* second iteration. */
      oprt = _mm256_cvtepi32_ps (_mm256_cvtepu8_epi32 (_mm_shuffle_epi32 (charged_t, 0b00001110)));
      oprm = _mm256_cvtepi32_ps (_mm256_cvtepu8_epi32 (_mm_shuffle_epi32 (charged_m, 0b00001110)));
     
      nat = _mm256_sub_ps (coefs_2, oprt);
      nam = _mm256_sub_ps (coefs_2, oprm);
      q = _mm256_rcp_ps (_mm256_add_ps (nat, nam));
      nat = _mm256_mul_ps (q, nat);
      nam = _mm256_mul_ps (q, nam);

      res = _mm256_cvtps_epi32 (_mm256_fmadd_ps (
        _mm256_shuffle_ps (nat, nat, 0b00000000), oprt, 
        _mm256_mul_ps (_mm256_shuffle_ps (nam, nam, 0b00000000), oprm)
      ));

      __m128i res2 = _mm_unpacklo_epi32 (
        _mm_shuffle_epi8 (_mm256_castsi256_si128 (res), mm_opr_mask),
        _mm_shuffle_epi8 (_mm256_extractf128_si256 (res, 1), mm_opr_mask)
      );

      _mm_stream_si128 ((__m128i*)buffer_this, _mm_or_si128 (
        _mm_and_si128 (
          _mm_min_epu8 (charged_t, charged_m), 
          _mm_set_epi8 (0,0,0,-1,0,0,0,-1,0,0,0,-1,0,0,0,-1)
        ),
        _mm_and_si128 (
          _mm_unpacklo_epi64 (res1, res2), 
          _mm_set_epi8 (-1,-1,-1,0,-1,-1,-1,0,-1,-1,-1,0,-1,-1,-1,0)
        )
      ));

      buffer_this++;
      buffer_mask++;
    }
  }

  return *this;
}
