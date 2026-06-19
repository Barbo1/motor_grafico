#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

Dir2 collision_point (const Square& sq1, const Square& sq2) {
  Dir2 pos1 = sq1.position, pos2 = sq2.position;
  float h1 = sq1.height, h2 = sq2.height; 
  float w1 = sq1.width, w2 = sq2.width; 
  Dir2 diff = pos1 - pos2;
  Dir2 size = Dir2 (w1 + w2, h1 + h2);

  bool cond = size.pL(diff.abs()) < 0.f;
  float q = static_cast<float>(cond);
  Dir2 n(1.f - q, q);
  Dir2 u = n * std::copysignf(1.f, diff * n);
  Dir2 aux(q, 1.f - q);
  Dir2 A, B, C, D;
  if (cond) {
    A = pos1 - u * h1 + aux * w1;
    B = pos1 - u * h1 - aux * w1;
    C = pos2 + u * h2 + aux * w2;
    D = pos2 + u * h2 - aux * w2;
  } else {
    A = pos1 - u * w1 + aux * h1;
    B = pos1 - u * w1 - aux * h1;
    C = pos2 + u * w2 + aux * h2;
    D = pos2 + u * w2 - aux * h2;
  }

  Dir2 v = B-A;
  return v.madd(Dir2(u.pLd(D-A, v), u.pLd(C-A, v)).bound01().sum() * 0.5f, A);
}
