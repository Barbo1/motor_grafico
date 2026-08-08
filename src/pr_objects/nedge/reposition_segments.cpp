#include "../../../headers/pr_objects/nedge.hpp"
#include "../../../headers/primitives/math.hpp"

void NEdge::reposition_segments() {
  Dir2 pos = this->position;
  __m128 mat_rotation = _mm_xor_ps(
    _mm_set_ps(0.f, 0.f, -0.f, 0.f),
    _mm_permute_ps(cos_sin_c(this->ang_pos).v, 0b00010100)
  );

  // calculate the placed points by rotating and adding the position.
  for (uint32_t i = 0; i < this->size; i++) {
    Dir2 sp = Dir2(triangles[i][0]);
    Dir2 sv = Dir2(triangles[(i+1) % (this->size)][0]) - sp;

    __m128 spr = _mm_mul_ps(mat_rotation, sp.v);
    __m128 svr = _mm_mul_ps(mat_rotation, sv.v);

    placed_segments[i][1].store(Dir2::from_well(_mm_hadd_ps(spr, spr)) + pos);
    placed_segments[i][0].store(Dir2::from_well(_mm_hadd_ps(svr, svr)));
  }
}
