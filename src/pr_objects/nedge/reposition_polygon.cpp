#include "../../../headers/pr_objects/nedge.hpp"
#include "../../../headers/primitives/math.hpp"

void NEdge::reposition_polygon() {
  Dir2 pos = this->position;
  __m128 mat_rotation = _mm_xor_ps(
    _mm_set_ps(0.f, 0.f, -0.f, 0.f),
    _mm_permute_ps(cos_sin_c(this->ang_pos).v, 0b00010100)
  );

  // calculate the placed points by rotating and adding the position.
  for (uint32_t i = 0; i < this->size - 2; i++) {
    Dir2 sp = Dir2(triangles[i][0]);
    Dir2 sv = Dir2(triangles[i+1][0]) - sp;
    Dir2 v1 = Dir2(triangles[i][1]);
    Dir2 v2 = Dir2(triangles[i][2]);

    __m128 spr = _mm_mul_ps(mat_rotation, sp.v);
    __m128 svr = _mm_mul_ps(mat_rotation, sv.v);
    __m128 v1r = _mm_mul_ps(mat_rotation, v1.v);
    __m128 v2r = _mm_mul_ps(mat_rotation, v2.v);

    Dir2 p = Dir2::from_well(_mm_hadd_ps(spr, spr)) + pos;

    placed_segments[i][1].store(p);
    placed_segments[i][0].store(Dir2::from_well(_mm_hadd_ps(svr, svr)));

    placed_triangles[i][0].store(p);
    placed_triangles[i][1].store(Dir2::from_well(_mm_hadd_ps(v1r, v1r)));
    placed_triangles[i][2].store(Dir2::from_well(_mm_hadd_ps(v2r, v2r)));
  }

  Dir2 pn2 = Dir2(triangles[this->size-2][0]);
  Dir2 pn1 = Dir2(triangles[this->size-1][0]);
  Dir2 p0 = Dir2(triangles[0][0]);
  Dir2 vn2 = pn1 - pn2;
  Dir2 vn1 = p0 - pn1;

  __m128 p1 = _mm_mul_ps(mat_rotation, pn2.v);
  __m128 v1 = _mm_mul_ps(mat_rotation, vn2.v);
  __m128 p2 = _mm_mul_ps(mat_rotation, pn1.v);
  __m128 v2 = _mm_mul_ps(mat_rotation, vn1.v);

  placed_segments[this->size-2][1].store(Dir2::from_well(_mm_hadd_ps(p1, p1)) + pos);
  placed_segments[this->size-2][0].store(Dir2::from_well(_mm_hadd_ps(v1, v1)));
  placed_segments[this->size-1][1].store(Dir2::from_well(_mm_hadd_ps(p2, p2)) + pos);
  placed_segments[this->size-1][0].store(Dir2::from_well(_mm_hadd_ps(v2, v2)));
}
