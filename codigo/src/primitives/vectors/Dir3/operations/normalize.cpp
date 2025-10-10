#include "../../../../../headers/primitives/vectors.hpp"
#include "../../../../../headers/primitives/operations.hpp"

Dir3 Dir3::normalize () const {
  float inorm = fisqrt(this->x * this->x + this->y * this->y + this->z * this->z);
  return Dir3 ({
    this->x * inorm,
    this->y * inorm,
    this->z * inorm
  });
}
