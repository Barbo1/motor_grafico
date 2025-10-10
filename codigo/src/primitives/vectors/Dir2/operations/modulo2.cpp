#include "../../../../../headers/primitives/vectors.hpp"
#include <cmath>

float Dir2::modulo2 () const {
  return this->x * this->x + this->y * this->y;
}
