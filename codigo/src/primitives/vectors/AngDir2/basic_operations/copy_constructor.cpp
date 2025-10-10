#include "../../../../../headers/primitives/vectors.hpp"

AngDir2::AngDir2 (const AngDir2 & adir) : Dir2 (adir) {
  this->a = adir.a;
}
