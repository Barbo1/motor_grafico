#include "../../../../headers/pr_objects/particle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/concepts/physical.hpp"

void resolve_collision (Particle& par, Square& sq) {
  float mass_1 = sq.get_mass(), mass_2 = par.mass;
  Dir2 par_pos = par.position;
  Dir2 par_vel  = par.velocity;
  Dir2 sq_pos = sq.position;
  Dir2 sq_vel = sq.velocity;

  Dir2 diff = sq_pos - par_pos;
  Dir2 b = diff.bound(Dir2 (sq.dims)) - diff;
  Dir2 n = b.normalize(); 
  float p = n * (sq_vel - par_vel) * 2.f / (mass_1 + mass_2);

  par.velocity.store(n.madd(p * mass_1 * ENERGY_DISIPATION, par_vel));
  par.position.store(n.msub(par.radio, b + par_pos));
}
