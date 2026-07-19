#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/concepts/physical.hpp"
#include "../../../../headers/pr_objects/particle.hpp"

void resolve_collition (Particle& par, Circle& cir1) {
  float mass_1 = cir1.get_mass();
  Dir2 par_pos = Dir2(par.position);
  Dir2 par_vel = Dir2(par.velocity);
  Dir2 n = (Dir2(cir1.position) - par_pos).normalize();
  float p = (Dir2(cir1.velocity) - par_vel) * n * 2.f / (mass_1 + par.mass);

  par.position.store(n.madd(cir1.radio + par.radio, par_pos));
  par.velocity.store(n.madd(p * mass_1 * ENERGY_DISIPATION, par_vel));
}
