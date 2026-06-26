#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

Dir2 collision_point (const Square& sq1, const Square& sq2) {
  Dir2 pos1 = sq1.position, pos2 = sq2.position;
  Dir2 sq1_dims = sq1.dims;
  Dir2 sq2_dims = sq2.dims;
  Dir2 diff = pos1 - pos2;
  Dir2 size = sq1_dims + sq2_dims;

  bool cond = size.pL(diff.abs()) < 0.f;
  float q = static_cast<float>(cond);
  Dir2 n(1.f - q, q);
  Dir2 u = n * std::copysignf(1.f, diff * n);
  Dir2 aux(q, 1.f - q);
  Dir2 A, B, C, D;
  if (cond) {
    A = pos1 - u * sq1_dims.y + aux * sq1_dims.x;
    B = pos1 - u * sq1_dims.y - aux * sq1_dims.x;
    C = pos2 + u * sq2_dims.y + aux * sq2_dims.x;
    D = pos2 + u * sq2_dims.y - aux * sq2_dims.x;
  } else {
    A = pos1 - u * sq1_dims.x + aux * sq1_dims.y;
    B = pos1 - u * sq1_dims.x - aux * sq1_dims.y;
    C = pos2 + u * sq2_dims.x + aux * sq2_dims.y;
    D = pos2 + u * sq2_dims.x - aux * sq2_dims.y;
  }

  Dir2 v = B-A;
  return v.madd(Dir2(u.pLd(D-A, v), u.pLd(C-A, v)).bound01().sum() * 0.5f, A);
}
