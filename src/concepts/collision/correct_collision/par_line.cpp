#include "../../../../headers/concepts/collision.hpp"
#include <cmath>

void correct_collision (Line& line, Particle& par) {
  Dir2 u = Dir2 (line.get_slope(), -1.f);
  float num = par.position.y - std::fmaf(line.get_slope(), par.position.x, line.get_deviation());
  float denom = std::fmaf(line.get_slope(), line.get_slope(), 1.f);
  u *= num / denom;
  par.position.store(Dir2(par.position) + u.normalize().nmadd(par.radio, u));
}
