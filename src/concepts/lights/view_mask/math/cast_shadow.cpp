#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <array>
#include <algorithm>
#include <cstdint>

void cast_shadow (Uint32*& buffer, int32_t width, int32_t height, const std::array<Dir2, 8>& points, uint32_t many_points) {

  /* Searching maximum and minimum coordenates. */
  float maxy = Dir2 {_mm_max_ps (points[0].v, _mm_max_ps (points[1].v, _mm_max_ps(points[2].v,
    _mm_max_ps(points[3].v, _mm_max_ps(points[4].v, points[5].v))
  )))}.y;

  float miny = Dir2{_mm_min_ps (points[0].v, _mm_min_ps (points[1].v, _mm_min_ps(points[2].v,
    _mm_min_ps(points[3].v, _mm_min_ps(points[4].v, points[5].v))
  )))}.y;

  auto bound_inside = [&] (int32_t num, int32_t up) {
    return std::max<int32_t> (std::min<int32_t> (num, up), 0);
  };

  int32_t top = bound_inside (std::lround(maxy), height) * width;
  int32_t bot = bound_inside (std::lround(miny), height);

  // Calculating coefitients.
  alignas(16) std::array<std::array<float, 4>, 6> coef = std::array<std::array<float, 4>, 6>();

  uint32_t many_segments = 0;
  for (std::size_t i = 1; i <= many_points; i++) {
    const Dir2 p1_p2 = points[i+1] - points[i];
    if (p1_p2.y != 0) {
      float q = 1.f / p1_p2.y;
      float mi = p1_p2.x * q;
      float ci = std::fmaf (-mi, points[i+1].y, points[i+1].x);
      float p = -points[i].y * q;
      coef[many_segments++] = {
        {mi, std::fmaf(mi, bot, ci), q, std::fmaf(q, bot, p)}
      };
    }
  }

  // Filling the shadows.
  __m128i color_mm = _mm_set1_epi32 (255);
  
  for (int32_t level = bot * width; level < top; level += width) {
    std::array<int32_t, 2> bounds = {-1, -1};
    uint32_t founded = 0;
    for (uint32_t i = 0; i < many_segments; i++) {
      auto& [dev, elem, p, q] = coef[i];
      if (-0.00001 < q && q < 1.00001) {
        int32_t inter = std::lround(elem);
        if (founded == 0 || (founded == 1 && bounds[0] != inter)) {
          bounds[founded++] = inter;
        }
      }
      q += p; elem += dev;
    }

    if (founded == 2) {
      const std::pair<int32_t, int32_t> min_max_res = std::minmax(bounds[0], bounds[1]);
      int32_t first = bound_inside (min_max_res.first - 1, width) + level;
      int32_t last = bound_inside (min_max_res.second + 1, width) + level;
      uint32_t many = last - first;

      __m128i* position_128 = (__m128i*)(buffer + first);
      for (uint32_t i = 0; i < many/4; i++, position_128++)
        _mm_storeu_si128 (position_128, color_mm);

      Uint32* position_32 = (Uint32*)position_128;
      for (uint32_t i = 0; i < many%4; position_32++, i++) {
        *position_32 = 255;
      }
    }
  }
}
