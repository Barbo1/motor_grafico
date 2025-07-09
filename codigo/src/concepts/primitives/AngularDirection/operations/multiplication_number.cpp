#include "../../../../../headers/concepts/primitives.hpp"

AngularDirection AngularDirection::operator* (float f) {
  return AngularDirection(this->x * f, this->y * f, this->a);
}
