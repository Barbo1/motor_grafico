#include "../../../../../headers/primitives/vectors.hpp"
#include <cmath>

float Dir3::modulo2 () const {
  return this->x * this->x + this->y * this->y + this->z * this->z;
}
