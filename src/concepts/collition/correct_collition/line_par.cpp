#include "../../../../headers/pr_objects/particle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

#include <cmath>

void correct_collition (Particle& par, Line& line) {
  AngDir2 u = AngDir2 {line.get_slope(), -1.f, par.get_position().a};
  float num = par.get_position().y - std::fmaf(line.get_slope(), par.get_position().x, line.get_deviation());
  float denom = std::fmaf(line.get_slope(), line.get_slope(), 1.f);
  u *= num / denom;
  par.add_position(u.normalize().nmadd(par.get_radio(), u));
}
