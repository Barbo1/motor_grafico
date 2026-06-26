#include "../../../../headers/pr_objects/line.hpp"

float Line::get_deviation () const {
  return Dir2(this->v).pLd(Dir2(this->p), Dir2 (0.f, 1.f));
}
