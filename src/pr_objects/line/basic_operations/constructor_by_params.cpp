#include "../../../../headers/pr_objects/line.hpp"

Line::Line (float slope, float deviation) noexcept {
  this->v = MemDir2 (1.f, -slope);
  this->p = MemDir2 (deviation, 0.f);
}
