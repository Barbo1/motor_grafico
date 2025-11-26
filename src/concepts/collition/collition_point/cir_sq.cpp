#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

Dir2 collition_point (Circle& cir, Square& sq) {
  return sq.get_position() - (
    sq.get_position() - cir.get_position()
  ).bound (AngDir2 {sq.get_width(), sq.get_height(), 0.f});
}
