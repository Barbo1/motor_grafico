#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

Dir2 collition_point (Square& sq, Line& line) {
  float h = sq.height * sgn(line.slope), 
        a = (line.slope * sq.position.x + line.deviation - sq.position.y) / 
            (line.slope * sq.width + h);
  AngDir2 t = AngDir2 {line.slope, -1.f, 0}.normalize();
  return sq.position + t * (t * (AngDir2 {sq.width, -h, 0} * (sgn(a) - a)));
}
