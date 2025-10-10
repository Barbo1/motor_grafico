#include "../../../../../headers/primitives/vectors.hpp"
#include <utility>

AngDir2::AngDir2 (AngDir2 && adir) : Dir2 (adir) {
  this->a = std::exchange(adir.a, 0);
}
