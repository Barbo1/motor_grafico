#include "../../../../../headers/primitives/vectors.hpp"
#include <cmath>

float Dir3::modulo () const {
  return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}
