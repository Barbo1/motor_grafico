#include "../../../../../headers/concepts/primitives.hpp"

AngDir2 AngDir2::normalize () {
  float inorm = 1/this->modulo();
  return AngDir2 (
    this->x * inorm,
    this->y * inorm,
    this->a
  );
}
