#include "../../../../headers/pr_objects/particle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

void correct_collision (Particle& par, Square& sq) {
  Dir2 diff = sq.get_position() - par.get_position();
  Dir2 b = diff.bound (Dir2 (sq.dims)) - diff;
  par.add_position (b.normalize().msub(par.get_radio(), b));
}
