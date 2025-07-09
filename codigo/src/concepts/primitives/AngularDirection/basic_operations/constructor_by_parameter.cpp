#include "../../../../../headers/concepts/primitives.hpp"

AngularDirection::AngularDirection (float x, float y, float a) : Direction (x, y) {
  this->a = a;
}
