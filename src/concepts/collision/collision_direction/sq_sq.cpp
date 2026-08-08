#include "../../../../headers/concepts/collision.hpp"

Dir2 collision_direction (Square& sq1, Square& sq2) {
  Dir2 diff = Dir2(sq1.position) + Dir2(sq2.position);
  Dir2 size = Dir2(sq1.dims) + Dir2(sq2.dims);

  bool cond = size.pL(diff.abs()) < 0.f;
  float q = static_cast<float>(cond);
  Dir2 n(1.f - q, q);
  return n * std::copysignf(1.f, diff * n);
}
