#include "../../../../../headers/concepts/primitives.hpp"

AngDir2 AngDir2::operator- () const {
  return AngDir2(-this->x, -this->y, this->a);
}
