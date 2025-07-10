#include "../../../../../headers/concepts/primitives.hpp"

bool AngDir2::operator== (const AngDir2 & adir) {
  return this->Dir2::operator==(adir) && this->a == adir.a;
}
