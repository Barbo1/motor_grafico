#include "../../../../../headers/concepts/primitives.hpp"
#include "../../utils/fisqrt.cpp"

float AngDir2::fangle () const {
  return (this->x + this->y) * fisqrt(this->x * this->x + this->y * this->y);
}
