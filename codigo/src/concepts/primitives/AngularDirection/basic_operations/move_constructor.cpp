#include "../../../../../headers/concepts/primitives.hpp"
#include <utility>

AngularDirection::AngularDirection (AngularDirection && adir) : Direction (adir) {
  this->a = std::exchange(adir.a, 0);
}
