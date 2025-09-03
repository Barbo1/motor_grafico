#include "../../../../../headers/concepts/primitives.hpp"

AngDir2 AngDir2::operator* (float f) const {
  return AngDir2(this->x * f, this->y * f, this->a);
}
