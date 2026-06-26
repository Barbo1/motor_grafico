#include "../../../../headers/pr_objects/line.hpp"

Line::Line (Dir2 p1, Dir2 p2) noexcept {
  this->v.store(p2 - p1);
  this->p.store(p1);
}
