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
  {
    __m128 max = _mm_max_ps(
      _mm_max_ps(points[0].v, _mm_max_ps(points[1].v, points[2].v)), 
      _mm_max_ps(points[3].v, _mm_max_ps(points[4].v, points[5].v))
    );
    __m128 min = _mm_min_ps(
      _mm_min_ps(points[0].v, _mm_min_ps(points[1].v, points[2].v)), 
      _mm_min_ps(points[3].v, _mm_min_ps(points[4].v, points[5].v))
    );
    __m128 both = _mm_movelh_ps(max, min);
    __m128i bothi = _mm_cvtps_epi32(both);

    top = std::clamp(_mm_extract_epi32(bothi, 1), 0, height) * width;
    bot = std::clamp(_mm_extract_epi32(bothi, 3), 0, height);
  }

  // Calculating coefitients.
  std::array<Dir2, 2> filtered_segments[6];
  uint32_t many_segments = 0;
  for (std::size_t i = 0; i < many_points; i++) {
    const Dir2 v = points[i+1] - points[i];
    if (std::abs(v.y()) > 1.f) {
      filtered_segments[many_segments][0] = v;
      filtered_segments[many_segments][1] = points[i];
      many_segments++;
    }
  }

  // Filling the shadows.
  __m128i color_mm = _mm_set1_epi32 (color);
  Dir2 C = Dir2(0.f, bot);
  for (int32_t level = bot * width; level < top; level += width, C += Dir2(0.f, 1.f)) {
    uint32_t founded = 0;
    std::array<int32_t, 2> bounds = {-1, -1};
    for (uint32_t i = 0; i < many_segments; i++) {
      Dir2 v = filtered_segments[i][0];
      Dir2 p = filtered_segments[i][1];
      float coef = Dir2::from_well(_mm_div_ps((C - p).v, v.v)).y();
      int32_t x = static_cast<int32_t>(v.madd(coef, p).x());
      if (-0.0001f < coef && coef < 1.0001f && (founded == 0 || (founded == 1 && bounds[0] != x))) {
        bounds[founded++] = x;
      }
    }

    if (founded == 2) {
      std::pair<int32_t, int32_t> min_max_res = std::minmax (bounds[0], bounds[1]);
      int32_t first = std::clamp (min_max_res.first - 1, 0, width);
      int32_t last = std::clamp (min_max_res.second + 1, 0, width);
      int32_t many = last - first;
      int32_t iter = 0;

      __m128i* position_128 = (__m128i*)(buffer + first + level);
      for (iter = 0; iter < (((many>>2)-1)<<2); iter+=4, position_128++)
        _mm_storeu_si128 (position_128, color_mm);

      Uint32* position_32 = (Uint32*)position_128;
      for (; iter < many; position_32++, iter++)
        *position_32 = color;
    }
  }
}
