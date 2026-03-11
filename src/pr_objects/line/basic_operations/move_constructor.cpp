#include "../../../../headers/pr_objects/line.hpp"
#include <utility>

Line::Line (Line && line) noexcept {
  this->v = std::exchange (line.v, Dir2 (1.f, 0.f));
  this->p = std::exchange (line.p, Dir2 (0.f, 0.f));
}
