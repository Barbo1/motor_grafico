#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/concepts/collition.hpp"

#include <cmath>

bool test_collition (const Line& line, const Circle& cir) {
  return 
    cir._colidalble && 
    std::abs(line.v.percan().normalize() * (line.p - cir.position)) < cir.radio;
}
