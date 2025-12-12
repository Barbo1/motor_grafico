#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_surface.h>
#include <array>

void fill_view_with_shadows (
  SDL_Surface*& img, const Dir2& position, const std::vector<MaskObject>& segments, const Uint32 color
) {
  Uint32* buffer = (Uint32*)img->pixels;

  const std::vector<MaskObject> viewed = generate_view_covering_by_point (position, segments);

  Dir2 dims = Dir2 {(float)img->w, (float)img->h};
  const Dir2& position_off = dims.nmadd (0.5f, position);

  std::array<Dir2, 8> points;
  for (const MaskObject& segment: viewed) {
    const Dir2 dir1_off = segment.point1 - position;
    const Dir2 dir2_off = segment.point2 - position;

    const Dir2 I_1 = Dir2 {_mm_xor_ps (_mm_and_ps (dir1_off.v, _mm_set1_ps (-0.f)), dims.v)};
    const Dir2 aux1 = Dir2 {_mm_div_ps ((I_1 - position_off).v, dir1_off.v)};
    Dir2 Q_1 = dir1_off * std::min (aux1.x, aux1.y);
    
    const Dir2 I_2 = Dir2 {_mm_xor_ps (_mm_and_ps (dir2_off.v, _mm_set1_ps (-0.f)), dims.v)};
    const Dir2 aux2 = Dir2 {_mm_div_ps ((I_2 - position_off).v, dir2_off.v)};
    Dir2 Q_2 = dir2_off * std::min (aux2.x, aux2.y);

    const float opr = dir2_off.pLd(Q_2 - Q_1, dir1_off);
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

    cast_shadow (buffer, (int32_t)img->w, (int32_t)img->h, points, 6, color);
  }
}
