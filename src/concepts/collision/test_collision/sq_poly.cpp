#include "../../../../headers/concepts/collision.hpp"

bool test_collision (const Square& sq, const NEdge& poly) {
  Dir2 sq_dims = Dir2 (sq.dims);
  Dir2 sq_pos = Dir2 (sq.position);
  for (uint32_t i = 0; i < poly.size - 2; i++) {
    const auto& triangle = poly.placed_triangles[i];
    const Dir2 A = triangle[0];
    const Dir2 vB = triangle[1];
    const Dir2 vC = triangle[2];
    Dir2 coefs;
    bool eval1 = test_collision_square_segment (sq_pos, sq_dims, A, vB, coefs);
    bool eval2 = test_collision_square_segment (sq_pos, sq_dims, A, vC, coefs);
    bool eval3 = test_collision_square_segment (sq_pos, sq_dims, A + vB, vC - vB, coefs);
    bool eval4 = test_collision_triangle_point (A, vB, vC, sq_pos);
    if (eval1 || eval2 || eval3 || eval4) {
      return true;
    }
  }
  return false;
}
