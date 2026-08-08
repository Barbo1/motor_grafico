#include "../../../../headers/concepts/collision.hpp"

void correct_collision (Particle& par, Square& sq) {
  Dir2 diff = Dir2(sq.position) - Dir2(par.position);
  Dir2 b = diff.bound (Dir2 (sq.dims)) - diff;
  par.position.store(Dir2(par.position) + b.normalize().msub(par.radio, b));
}
