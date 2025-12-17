#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/particle.hpp"

#include <cmath>

void resolve_collition (Particle& par, Line& line) {
  AngDir2 u = AngDir2 {line.get_slope(), -1.f, par.get_position().a};
  float num = par.get_position().y - std::fmaf(line.get_slope(), par.get_position().x, line.get_deviation());
  float denom = std::fmaf(line.get_slope(), line.get_slope(), 1.f);
  u *= num / denom;
  AngDir2 n = u.normalize();

  par.add_velocity(n * (-2.f * (n * par.get_velocity())));
  par.add_position(n.nmadd(par.get_radio(), u));
}
