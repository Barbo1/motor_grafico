#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/particle.hpp"

void resolve_collition (Particle& par, Circle& cir1) {
  float mass_1 = cir1.get_mass(), mass_2 = par.get_mass();
  AngDir2 n = (cir1.get_position() - par.get_position()).normalize();
  float p = (cir1.get_velocity() - par.get_velocity()) * n * 2.f / (mass_1 + mass_2);

  par.set_velocity(par.get_velocity() + n * (p * mass_1));
  par.set_position(par.get_position() + n * (cir1.get_radio() + par.get_radio()));
}
