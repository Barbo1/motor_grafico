#include "../../../../headers/concepts/collision.hpp"
#include <cmath>

void resolve_collision (Line& line, Particle& par) {
  Dir2 par_vel = Dir2(par.velocity);

  Dir2 u = Dir2 (line.get_slope(), -1.f);
  float num = par.position.y - std::fmaf(line.get_slope(), par.position.x, line.get_deviation());
  float denom = std::fmaf(line.get_slope(), line.get_slope(), 1.f);
  u *= num / denom;
  Dir2 n = u.normalize();

  par.position.store(n.nmadd(par.radio, u + Dir2(par.position)));
  par.velocity.store(n.madd(-2.f * (n * par_vel), par_vel));
}
