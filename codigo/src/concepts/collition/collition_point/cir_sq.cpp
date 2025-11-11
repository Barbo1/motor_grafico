#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

Dir2 collition_point (Circle& cir, Square& sq) {
  return Dir2 {
    sq.position.x - bound (sq.position.x - cir.position.x, sq.width),
    sq.position.y - bound (sq.position.y - cir.position.y, sq.height)
  };
}
