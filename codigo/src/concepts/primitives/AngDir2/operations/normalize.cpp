#include "../../../../../headers/concepts/primitives.hpp"
#include "../../../../../headers/concepts/operations.hpp"

AngDir2 AngDir2::normalize () {
  float inorm = fisqrt(this->x * this->x + this->y * this->y);
  return AngDir2 (
    this->x * inorm,
    this->y * inorm,
    this->a
  );
}
