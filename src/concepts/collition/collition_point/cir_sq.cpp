#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

Dir2 collition_point (const Circle& cir, const Square& sq) {
  return sq.position - (sq.position - cir.position).bound (AngDir2 {sq.width, sq.height, 0.f});
}
