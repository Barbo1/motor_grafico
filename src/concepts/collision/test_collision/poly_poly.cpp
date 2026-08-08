#include "../../../../headers/concepts/collision.hpp"

bool test_collision (const NEdge& poly1, const NEdge& poly2) {
  for (uint32_t i1 = 0; i1 < poly1.size-2; i1++) {
    const auto& triangle1 = poly1.placed_triangles[i1];
    const Dir2 A = triangle1[0];
    const Dir2 vB = triangle1[1];
    const Dir2 vC = triangle1[2];
    for (uint32_t i2 = 0; i2 < poly2.size-2; i2++) {
      const auto& triangle2 = poly2.placed_triangles[i2];
      const Dir2 D = triangle2[0];
      const Dir2 vE = triangle2[1];
      const Dir2 vF = triangle2[2];
      if (test_collision_triangle_triangle(A, vB, vC, D, vE, vF)) [[unlikely]] {
        return true;
      }
    }
  }
  return false;
}
