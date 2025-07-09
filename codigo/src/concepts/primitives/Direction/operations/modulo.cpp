#include "../../../../../headers/concepts/primitives.hpp"
#include <cmath>

float Direction::modulo () {
  return std::sqrt(this->x * this->x + this->y * this->y);
}
