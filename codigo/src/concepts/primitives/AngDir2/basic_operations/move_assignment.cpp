#include "../../../../../headers/concepts/primitives.hpp"
#include <utility>

AngDir2 & AngDir2::operator= (AngDir2 && adir) {
  this->Dir2::operator=(std::move(adir));
  this->a = adir.a;
  return *this;
}
