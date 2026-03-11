#include "../../../../headers/pr_objects/line.hpp"

float Line::get_deviation () const {
  return this->v.pLd(this->p, Dir2 (0.f, 1.f));
}
