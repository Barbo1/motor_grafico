#include "../../../../headers/pr_objects/particle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

void correct_collition (Particle& par, Line& line) {
  AngDir2&& u = AngDir2 {line.get_slope(), -1.f, par.get_position().a};
  u *= (par.get_position().y - (line.get_slope() * par.get_position().x + line.get_deviation())) / 
    (line.get_slope() * line.get_slope() + 1.f);
  AngDir2 n = u.normalize();
  par.set_position(par.get_position() + u - n * par.get_radio());
}
