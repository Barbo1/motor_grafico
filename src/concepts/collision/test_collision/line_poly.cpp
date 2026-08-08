#include "../../../../headers/concepts/collision.hpp"

bool test_collision (const Line& line, const NEdge& poly) {
  for (uint32_t i = 0; i < poly.size-2; i++) {
    const auto& triangle = poly.placed_triangles[i];
    const Dir2 A = Dir2(triangle[0]);
    const Dir2 vB = Dir2(triangle[1]);
    const Dir2 vC = Dir2(triangle[2]);
    const Dir2 vL = Dir2(line.v).percan();
    const float cond1 = vL * A;
    const float cond2 = vL * (A + vB);
    const float cond3 = vL * (A + vC);
    if (cond1*cond2 < 0.f || cond1*cond3 < 0.f) [[unlikely]] {
      return true;
    }
  }
  return false;
}

