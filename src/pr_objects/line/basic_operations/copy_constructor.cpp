#include "../../../../headers/pr_objects/line.hpp"

Line::Line (const Line & line) noexcept {
  this->v = line.v;
  this->p = line.p;
}
