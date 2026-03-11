#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

#include <cmath>

bool test_collition (Circle& cir, Line& line) {
  return 
    cir._colidalble && 
    std::abs(line.v.percan().normalize() * (line.p - cir.position)) < cir.radio;
}
