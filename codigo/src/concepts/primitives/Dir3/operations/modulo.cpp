#include "../../../../../headers/concepts/primitives.hpp"
#include <cmath>

float Dir3::modulo () {
  return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}
