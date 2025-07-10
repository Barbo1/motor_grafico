#include "../../../../../headers/concepts/primitives.hpp"
#include <cmath>

float Dir2::modulo () {
  return std::sqrt(this->x * this->x + this->y * this->y);
}
