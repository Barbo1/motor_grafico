#include "../../../../headers/concepts/collision.hpp"

bool test_collision (const Particle& par, const NEdge& poly) {
  for (uint32_t i = 0; i < poly.size-2; i++) {
    const auto& triangle = poly.placed_triangles[i];
    const Dir2 A = triangle[0];
    const Dir2 vB = triangle[1];
    const Dir2 vC = triangle[2];
    if (test_collision_triangle_circle(A, vB, vC, par.position, par.radio)) [[unlikely]] {
      return true;
    }
  }
  return false;
}

