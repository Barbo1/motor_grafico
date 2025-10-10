#include "../../../../../headers/primitives/vectors.hpp"

AngDir2 & AngDir2::operator= (const AngDir2 & adir) {
  this->Dir2::operator=(adir);
  this->a = adir.a;
  return *this;
}
