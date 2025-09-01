#include "../../../../../headers/concepts/primitives.hpp"
#include "../../utils/fisqrt.cpp"

Dir3 Dir3::normalize () const {
  float inorm = fisqrt(this->x * this->x + this->y * this->y + this->z * this->z);
  return Dir3 ({
    this->x * inorm,
    this->y * inorm,
    this->z * inorm
  });
}
