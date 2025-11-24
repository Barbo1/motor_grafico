#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/particle.hpp"

#include <cmath>

bool test_collition (Particle& par, Line& line) {
  float p1 = par.get_position().y - std::fmaf(line.get_slope(), par.get_position().x, line.get_deviation());
  float p2 = std::fmaf(line.get_slope(), line.get_slope(), 1.f);
  return p1 * p1 < par.get_radio() * par.get_radio() * p2;
}
