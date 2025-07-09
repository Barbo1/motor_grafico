#include "../../../../../headers/concepts/primitives.hpp"
#include <utility>

AngularDirection & AngularDirection::operator= (AngularDirection && adir) {
  this->Direction::operator=(std::move(adir));
  this->a = adir.a;
  return *this;
}
