#include "../../../../headers/pr_objects/line.hpp"

Line & Line::operator= (const Line & line) noexcept {
  this->v = line.v;
  this->p = line.p;
  return *this;
}
