#include "../../../../headers/pr_objects/square.hpp"

bool test_collition (Square& sq1, Square& sq2) {
  Dir2 diff = (sq1.position - sq2.position).abs();
  return sq1._colidalble && 
    sq2._colidalble && 
    diff.y < sq1.height + sq2.height && 
    diff.x < sq1.width + sq2.width;
}
