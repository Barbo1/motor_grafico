#include "../../../../headers/concepts/lights.hpp"

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <array>

void fill_view_with_shadows (
  Uint32* buffer, 
  int32_t width, 
  int32_t height, 
  DynamicalArena& darena,
  const MaskObjectList& segments,
  const Dir2& position,
  Uint32 color
) {
  if (segments.size == 0)
    return;

  MaskObjectList viewed = generate_view_covering (
    darena,
    segments,
    position,
    ViewGeneration::VG_POINT
  );

  const Dir2 dims = Dir2 (width, height);
  const Dir2& position_off = dims.nmadd (0.5f, position);

  __m128 neg_mm = _mm_set1_ps (-0.f);
  std::array<Dir2, 8> points;

  for (MaskObject* iter = viewed.obj; iter != nullptr; iter = iter->next) {
    const Dir2 dir1_off = Dir2(iter->point1) - position;
    const Dir2 dir2_off = Dir2(iter->point2) - position;

    const Dir2 I_1 = Dir2 (_mm_xor_ps (_mm_and_ps (dir1_off.v, neg_mm), dims.v));
    const Dir2 aux1 = Dir2 (_mm_div_ps ((I_1 - position_off).v, dir1_off.v));
    Dir2 Q_1 = dir1_off * std::min (aux1.x(), aux1.y());
    
    const Dir2 I_2 = Dir2 (_mm_xor_ps (_mm_and_ps (dir2_off.v, neg_mm), dims.v));
    const Dir2 aux2 = Dir2 (_mm_div_ps ((I_2 - position_off).v, dir2_off.v));
    Dir2 Q_2 = dir2_off * std::min (aux2.x(), aux2.y());

    const float opr = (dir2_off * (Q_2 - Q_1)) / dir2_off.pL(dir1_off);
    const float mod = dir1_off.percan().madd(opr, Q_1).modulo();
    const Dir2 Q_off = (dir1_off.normalize() + dir2_off.normalize()) * mod;

    Q_1 += position;
    Q_2 += position;

    points[0] = Dir2(iter->point1);
    points[1] = Q_1;
    points[2] = Q_off + Q_1;
    points[3] = Q_off + Q_2;
    points[4] = Q_2;
    points[5] = Dir2(iter->point2);
    points[6] = points[0];
    points[7] = points[1];

    cast_shadow (buffer, width, height, points, 6, color);
  }

  darena.complete_free_mo(viewed.obj);
}
