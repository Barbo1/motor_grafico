#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <array>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <optional>

void cast_shadow (Uint32*& buffer, int32_t width, int32_t height, std::array<Dir2, 8> points, Uint32 color) {

  /* Searching maximum and minimum coordenates. */
  float miny = points[0].y, maxy = points[0].y;
  for (auto& point: points) {
    if (maxy < point.y) maxy = point.y;
    if (miny > point.y) miny = point.y;
  }

  // Calculating coefitients.
  using coefType = std::pair<std::array<float, 4>, bool>;
  std::array<coefType, 6> coef = std::array<coefType, 6>();

  uint32_t many_segments = 0;
  for (std::size_t i = 1; i <= 6; i++) {
    const Dir2 p1_p2 = points[i+1] - points[i];
    if (p1_p2.y != 0) {
      float q = 1.f / p1_p2.y;
      float mi = p1_p2.x * q;
      float ci = points[i+1].x - points[i+1].y * mi;
      float p = -points[i].y * q;
      coef[many_segments++] = {
        {mi, ci, q, p}, 
        (points[i].y - points[i-1].y) * p1_p2.y > 0
      };
    }
  }

  std::array<std::optional<int32_t>, 2> bounds = std::array<std::optional<int32_t>, 2>();

  auto bound_inside = [&] (int32_t num, int32_t up) {
    return std::max<int32_t> (std::min<int32_t> (num, up), 0);
  };
  
  auto is_value = [&] (int32_t inter) {
    return
      (bounds[0].has_value() && bounds[0].value() == inter) || 
      (bounds[1].has_value() && bounds[1].value() == inter);
  };

  int32_t top = bound_inside (maxy, height) * width;
  int32_t bot = bound_inside (miny, height);
  if (bot > 0) {
    for (uint32_t i = 0; i < many_segments; i++) {
      auto &mc = coef[i];
      mc.first[3] += mc.first[2] * bot;
      mc.first[1] += mc.first[0] * bot;
    }
  }

  // Filling the shadows.
  __m128i color_mm = _mm_set1_epi32 (color);
  for (int32_t level = bot * width; level < top; level += width) {
    bounds = {std::optional<int32_t>(), std::optional<int32_t>()};
    uint32_t founded = 0;
    for (uint32_t i = 0; i < many_segments; i++) {
      auto &mc = coef[i];
      float &q = mc.first[3], &elem = mc.first[1];
      if (-0.00001 < q && q < 1.00001) {
        int32_t inter = std::lround(elem);
        if (is_value(inter)) 
          goto FIN;
        bounds[founded++] = inter;
      }
      FIN:
      q += mc.first[2]; elem += mc.first[0];
    }

    if (founded == 2) {
      const std::pair<int32_t, int32_t> min_max_res = std::minmax(bounds[0].value(), bounds[1].value());
      int32_t first = bound_inside (min_max_res.first, width) + level;
      int32_t last = bound_inside (min_max_res.second, width) + level;
      uint32_t many = (last - first);

      __m128i* position_128 = (__m128i*)(buffer + first);
      for (uint32_t i = 0; i < many/4; i++, position_128++)
        _mm_storeu_si128 (position_128, color_mm);

      Uint32* position_32 = (Uint32*)position_128;
      for (uint32_t i = 0; i < many%4; position_32++, i++) {
        *position_32 = color;
      }
    }
  }
}

ViewMask ViewMask::create_view_mask_by_point (
  const uint32_t& width, const uint32_t& height, 
  Dir2 position, const std::vector<MaskObject>& segments,
  const Uint32 color
) {
  const std::vector<MaskObject> viewed = generate_view_covering_by_point(position, segments);

  Uint32* buffer = static_cast<Uint32*>(std::aligned_alloc(16, width * height * sizeof (Uint32)));

  Dir2 dims = Dir2 {(float)width, (float)height};
  Dir2 dims2 = dims * 0.5f;
  const Dir2& position_off = position - dims2;

  std::array<Dir2, 8> points;
  for (const MaskObject& segment: viewed) {
    const Dir2 dir1_off = segment.point1 - position;
    const Dir2 dir2_off = segment.point2 - position;

    const Dir2 I_1 = Dir2 {_mm_xor_ps (_mm_and_ps (dir1_off.v, _mm_set1_ps (-0.f)), dims.v)};
    const Dir2 aux1 = Dir2 {_mm_mul_ps ((I_1 - position_off).v, _mm_rcp_ps (dir1_off.v))};
    Dir2 Q_1 = dir1_off * std::min (aux1.x, aux1.y);
    
    const Dir2 I_2 = Dir2 {_mm_xor_ps (_mm_and_ps (dir2_off.v, _mm_set1_ps (-0.f)), dims.v)};
    const Dir2 aux2 = Dir2 {_mm_mul_ps ((I_2 - position_off).v, _mm_rcp_ps (dir2_off.v))};
    Dir2 Q_2 = dir2_off * std::min (aux2.x, aux2.y);

    const float opr = (dir2_off * (Q_2 - Q_1)) / dir2_off.pL(dir1_off);
    const float mod = dir1_off.percan().madd(opr, Q_1).modulo();

    const Dir2 Q_off = (dir1_off.normalize() + dir2_off.normalize()) * mod;
    Q_1 += position;
    Q_2 += position;

    points[0] = segment.point1;
    points[1] = Q_1;
    points[2] = Q_off + Q_1;
    points[3] = Q_off + Q_2;
    points[4] = Q_2;
    points[5] = segment.point2;
    points[6] = points[0];
    points[7] = points[1];

    cast_shadow (buffer, width, height, points, color);
  }

  return ViewMask (SDL_CreateRGBSurfaceFrom (
    buffer, width, height, 32, width * 4, 
    0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
  ));
}
