#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/particle.hpp"

void resolve_collition (Particle& par, Circle& cir1) {
  float mass_1 = cir1.get_mass();
  AngDir2 n = (cir1.get_position() - par.get_position()).normalize();
  float p = (cir1.get_velocity() - par.get_velocity()) * n * 2.f / (mass_1 + par.get_mass());

  par.add_velocity(n * (p * mass_1 * ENERGY_DISIPATION));
  par.add_position(n * (cir1.get_radio() + par.get_radio()));
}
