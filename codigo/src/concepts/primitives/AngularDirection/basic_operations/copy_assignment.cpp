#include "../../../../../headers/concepts/primitives.hpp"

AngularDirection & AngularDirection::operator= (const AngularDirection & adir) {
  this->Direction::operator=(adir);
  this->a = adir.a;
  return *this;
}
