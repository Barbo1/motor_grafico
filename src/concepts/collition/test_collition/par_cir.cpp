#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/particle.hpp"

bool test_collition (const Particle& par, const Circle & cir) {
  uint32_t radio = cir.radio + par._radio;
  return (cir.get_position() - par.get_position()).modulo2() < radio * radio;
}
