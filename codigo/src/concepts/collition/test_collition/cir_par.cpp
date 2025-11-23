#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/particle.hpp"

bool test_collition (Particle& par, Circle & cir) {
  uint32_t radio = cir.get_radio() + par.get_radio();
  return (cir.get_position() - par.get_position()).modulo2() < radio * radio;
}
