#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

Dir2 collition_point (Square& sq, Line& line) {
  float h = sq.get_height() * sgn(line.get_slope()), 
        a = (line.get_slope() * sq.get_position().x + line.get_deviation() - sq.get_position().y) / 
            (line.get_slope() * sq.get_width() + h);
  AngDir2 t = AngDir2 {line.get_slope(), -1.f, 0}.normalize();
  return sq.get_position() + t * (t * (AngDir2 {sq.get_width(), -h, 0} * (sgn(a) - a)));
}
