#include "../../../../../headers/concepts/primitives.hpp"
#include "../../utils/fisqrt.cpp"

Direction Direction::normalize () {
  float inorm = fisqrt(this->x * this->x + this->y * this->y);
  return Direction ({
    this->x * inorm,
    this->y * inorm
  });
}
