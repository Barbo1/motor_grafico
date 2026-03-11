#include "../../../../headers/pr_objects/line.hpp"

Line::Line (float slope, float deviation) noexcept {
  this->v = Dir2 (1.f, -slope);
  this->p = Dir2 (deviation, 0.f);
}
