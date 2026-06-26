#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/particle.hpp"

#include <cmath>

bool test_collision (const Line& line, const Particle& par) {
  float p1 = par.position.y - std::fmaf(line.get_slope(), par.position.x, line.get_deviation());
  float p2 = std::fmaf(line.get_slope(), line.get_slope(), 1.f);
  return p1 * p1 < par.radio * par.radio * p2;
}
