#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/particle.hpp"

bool test_collision (const Particle& par, const Circle & cir) {
  uint32_t radio = cir.radio + par.radio;
  return (Dir2(cir.position) - Dir2(par.position)).modulo2() < radio * radio;
}
