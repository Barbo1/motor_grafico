#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

Dir2 collition_point (Square& sq, Circle& cir) {
  return Dir2 {
    sq.get_position().x - bound (sq.get_position().x - cir.get_position().x, sq.get_width()),
    sq.get_position().y - bound (sq.get_position().y - cir.get_position().y, sq.get_height())
  };
}
