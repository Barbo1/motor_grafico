#include "../../../../../headers/concepts/primitives.hpp"
#include "../../utils/fisqrt.cpp"

AngDir2 AngDir2::fnormalize () {
  float inorm = fisqrt(this->x * this->x + this->y * this->y);
  return AngDir2 (
    this->x * inorm,
    this->y * inorm,
    this->a
  );
}
