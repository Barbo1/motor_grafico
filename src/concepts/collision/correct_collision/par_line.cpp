#include "../../../../headers/pr_objects/particle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

#include <cmath>

void correct_collision (Particle& par, Line& line) {
  Dir2 u = Dir2 (line.get_slope(), -1.f);
  float num = par.position.y - std::fmaf(line.get_slope(), par.position.x, line.get_deviation());
  float denom = std::fmaf(line.get_slope(), line.get_slope(), 1.f);
  u *= num / denom;
  par.position.store(Dir2(par.position) + u.normalize().nmadd(par.radio, u));
}
