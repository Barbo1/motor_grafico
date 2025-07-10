#include "../../../../../headers/concepts/primitives.hpp"
#include <utility>

AngDir2::AngDir2 (AngDir2 && adir) : Dir2 (adir) {
  this->a = std::exchange(adir.a, 0);
}
