#include "../../../../headers/pr_objects/line.hpp"
#include <utility>

Line & Line::operator= (Line && line) noexcept {
  this->v = std::exchange (line.v, MemDir2(1.f, 0.f));
  this->p = std::exchange (line.p, MemDir2());
  return *this;
}
