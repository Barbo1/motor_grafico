#include "../../../../headers/concepts/collision.hpp"

bool test_collision (const Circle& cir, const NEdge& poly) {
  for (uint32_t i = 0; i < poly.size-2; i++) {
    const auto& [A, vB, vC] = poly.placed_triangles[i];
    if (test_collision_triangle_circle(Dir2(A), Dir2(vB), Dir2(vC), cir.position, cir.radio)) {
      return true;
    }
  }
  return false;
}
