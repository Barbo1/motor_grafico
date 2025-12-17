#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

#include <cmath>

void resolve_collition (Square& sq1, Square& sq2) {
  float mass_1 = sq1.get_mass(), mass_2 = sq2.get_mass();
  AngDir2 diff = sq1.position - sq2.position;
  AngDir2 size = Dir2 {sq1.width + sq2.width, sq2.height + sq1.height};

  float q = static_cast<float>(size.pL(diff.abs()) < 0.f);
  AngDir2 n(1.f - q, q, 0.f);
  float res = diff * n;
  AngDir2 ns = n * std::copysign(1.f, res);
  float p = ns * (sq1._velocity - sq2._velocity) * 2.f / (mass_1 + mass_2);

  sq1._velocity = ns.nmadd (p * mass_2 * sq1._movible, sq1._velocity);
  sq2._velocity = ns.madd (p * mass_1 * sq2._movible, sq2._velocity);

  sq1.position = n.madd (size * ns - res, sq1.position);

  sq1._collition_normal = ns;
  sq2._collition_normal = -ns;

  sq1._acc_f_k = sq1._f_k * sq2._f_k;
  sq2._acc_f_k = sq1._acc_f_k;

  sq1._normal_presence = true;
  sq2._normal_presence = true;
}
