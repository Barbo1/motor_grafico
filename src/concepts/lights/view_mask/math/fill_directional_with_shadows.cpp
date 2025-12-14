#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_surface.h>
#include <array>

void fill_directional_with_shadows (
  SDL_Surface*& img, const Dir2& direction, const std::vector<MaskObject>& segments, const Uint32 color
) {
  Uint32* buffer = (Uint32*)img->pixels;

  const std::vector<MaskObject> viewed = generate_view_covering_by_direction (direction, segments);

  const Dir2 dims = Dir2 {(float)img->w, (float)img->h};
  const Dir2 dims2 = dims * 0.5f;
  const Dir2 I = Dir2 {_mm_xor_ps (_mm_and_ps (direction.v, _mm_set1_ps (-0.f)), dims.v)} + dims2;

  std::array<Dir2, 8> points;
  for (const MaskObject& segment: viewed) {

    __m128 denom = _mm_rcp_ps (direction.v);
    __m128 aux1 = _mm_mul_ps ((I - segment.point1).v, denom);
    __m128 aux2 = _mm_mul_ps ((I - segment.point2).v, denom);
    float coef1 = _mm_cvtss_f32 (_mm_min_ps (aux1, _mm_shuffle_ps (aux1, aux1, 0b00000001)));
    float coef2 = _mm_cvtss_f32 (_mm_min_ps (aux2, _mm_shuffle_ps (aux2, aux2, 0b00000001)));

    points[0] = segment.point1;
    points[1] = direction.madd (coef1, segment.point1);
    points[2] = direction.madd (coef2, segment.point2);
    points[3] = segment.point2;
    points[4] = points[0];
    points[5] = points[1];

    cast_shadow (buffer, (int32_t)img->w, (int32_t)img->h, points, 4, color);
  }
}
