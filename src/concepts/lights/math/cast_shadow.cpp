#include "../../../../headers/concepts/lights.hpp"

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <array>
#include <algorithm>
#include <cstdint>

void cast_shadow (
  Uint32* buffer, 
  int32_t width, 
  int32_t height, 
  const std::array<Dir2, 8>& points, 
  uint32_t many_points,
  Uint32 color
) {
  int32_t top, bot;
  float botf;
  {
    __m128 max = _mm_max_ps(
      _mm_max_ps(points[0].v, _mm_max_ps(points[1].v, points[2].v)), 
      _mm_max_ps(points[3].v, _mm_max_ps(points[4].v, points[5].v))
    );
    __m128 min = _mm_min_ps(
      _mm_min_ps(points[0].v, _mm_min_ps(points[1].v, points[2].v)), 
      _mm_min_ps(points[3].v, _mm_min_ps(points[4].v, points[5].v))
    );
    __m128 both = _mm_max_ps(_mm_min_ps(_mm_movelh_ps(max, min), _mm_set1_ps(height)), _mm_setzero_ps());
    __m128i bothi = _mm_cvtps_epi32(both);

    botf = _mm_cvtss_f32(_mm_permute_ps(both, 0b11));
    top = _mm_extract_epi32(bothi, 1) * width;
    bot = _mm_extract_epi32(bothi, 3);
  }

  auto bound_inside = [&] (int32_t num, int32_t up) {
    return std::max<int32_t> (std::min<int32_t> (num, up), 0);
  };

  // Calculating coefitients.
  __m128 coef[6];
  __m128 coef_sum[6];
  uint32_t many_segments = 0;
  for (std::size_t i = 0; i < many_points; i++) {
    const Dir2 p1_p2 = points[i+1] - points[i];
    if (p1_p2.y() != 0) {
      float q = 1.f / p1_p2.y();
      float mi = p1_p2.x() * q;
      _mm_storeu_ps(reinterpret_cast<float*>(coef_sum + many_segments), _mm_set_ps(0.f, 0.f, q, mi));
      _mm_storeu_ps(
        reinterpret_cast<float*>(coef + many_segments), 
        _mm_set_ps(
          0.f, 
          0.f, 
          q * (botf - points[i].y()),
          std::fmaf(mi, botf - points[i+1].y(), points[i+1].x())
        )
      );
      many_segments++;
    }
  }

  // Filling the shadows.
  __m128i color_mm = _mm_set1_epi32 (color);
  __m128 cond_bound_top = _mm_set1_ps(1.0001f);
  __m128 cond_bound_bot = _mm_set1_ps(-0.0001f);
  for (int32_t level = bot * width; level < top; level += width) {
    std::array<int32_t, 2> bounds = {-1, -1};
    uint32_t founded = 0;
    for (uint32_t i = 0; i < many_segments; i++) {
      __m128 part = _mm_loadu_ps(reinterpret_cast<float*>(coef + i));
      __m128 cond = _mm_and_ps(_mm_cmplt_ps(part, cond_bound_top), _mm_cmplt_ps(cond_bound_bot, part));
      if (_mm_movemask_ps(cond) & 0b10) {
        int32_t inter = _mm_cvt_ss2si(part);
        if (founded == 0 || (founded == 1 && bounds[0] != inter)) {
          bounds[founded++] = inter;
        }
      }
      __m128 part_sum = _mm_loadu_ps(reinterpret_cast<float*>(coef_sum + i));
      _mm_storeu_ps(reinterpret_cast<float*>(coef + i), _mm_add_ps(part, part_sum));
    }
    
    if (founded == 2) {
      const std::pair<int32_t, int32_t> min_max_res = std::minmax(bounds[0], bounds[1]);
      int32_t first = bound_inside (min_max_res.first - 1, width);
      int32_t last = bound_inside (min_max_res.second + 1, width);
      uint32_t many = last - first;

      __m128i* position_128 = (__m128i*)(buffer + first + level);
      for (uint32_t i = 0; i < many/4; i++, position_128++)
        _mm_storeu_si128 (position_128, color_mm);

      Uint32* position_32 = (Uint32*)position_128;
      for (uint32_t i = 0; i < many%4; position_32++, i++) {
        *position_32 = color;
      }
    }
  }
}
