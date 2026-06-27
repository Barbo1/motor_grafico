#include "../../../../headers/pr_objects/particle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

bool test_collision (const Particle& par, const Square& sq) {
  Dir2 diff = ((Dir2(par.position) - Dir2(sq.position)).abs() - Dir2(sq.dims)).max0();
  return diff.modulo2() < par.radio * par.radio;
}
