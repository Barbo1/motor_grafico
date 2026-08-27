#include "../../../../headers/concepts/lights.hpp"

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <array>
#include <cstdint>

void fill_directional_with_shadows (
  Uint32* buffer, 
  int32_t width, 
  int32_t height, 
  Arena& arena,
  DynamicalArena& darena,
  const MaskObjectList& segments,
  const Dir2& direction,
  Uint32 color
) {
  MaskObjectList viewed = generate_view_covering (
    arena,
    darena,
    segments,
    direction,
    ViewGeneration::VG_DIRECTION
  );

  const Dir2 dims = Dir2 (width, height);
  const Dir2 dims2 = dims * 0.5f;
  const Dir2 I = Dir2 (_mm_xor_ps (_mm_and_ps (direction.v, _mm_set1_ps (-0.f)), dims.v)) + dims2;

  std::array<Dir2, 8> points;
  for (MaskObject* iter = viewed.obj; iter != nullptr; iter = iter->next) {
    Dir2 seg_point_1 = Dir2(iter->point1);
    Dir2 seg_point_2 = Dir2(iter->point2);

    __m128 denom = _mm_rcp_ps (direction.v);
    __m128 aux1 = _mm_mul_ps ((I - seg_point_1).v, denom);
    __m128 aux2 = _mm_mul_ps ((I - seg_point_2).v, denom);
    float coef1 = _mm_cvtss_f32 (_mm_min_ps (aux1, _mm_shuffle_ps (aux1, aux1, 0b00000001)));
    float coef2 = _mm_cvtss_f32 (_mm_min_ps (aux2, _mm_shuffle_ps (aux2, aux2, 0b00000001)));

    points[0] = seg_point_1;
    points[1] = direction.madd (coef1, seg_point_1);
    points[2] = direction.madd (coef2, seg_point_2);
    points[3] = seg_point_2;
    points[4] = points[0];
    points[5] = points[1];

    cast_shadow (buffer, width, height, points, 4, color);
  }

  darena.complete_free_mo(viewed.obj);
}
