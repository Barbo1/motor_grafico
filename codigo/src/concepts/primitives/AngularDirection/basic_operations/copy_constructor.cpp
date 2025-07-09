#include "../../../../../headers/concepts/primitives.hpp"

AngularDirection::AngularDirection (const AngularDirection & adir) : Direction (adir) {
  this->a = adir.a;
}
