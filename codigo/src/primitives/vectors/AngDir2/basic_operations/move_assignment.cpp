#include "../../../../../headers/primitives/vectors.hpp"
#include <utility>

AngDir2 & AngDir2::operator= (AngDir2 && adir) {
  this->Dir2::operator=(std::move(adir));
  this->a = adir.a;
  return *this;
}
