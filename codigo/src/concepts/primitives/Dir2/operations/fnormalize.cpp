#include "../../../../../headers/concepts/primitives.hpp"
#include "../../utils/fisqrt.cpp"

Dir2 Dir2::fnormalize () {
  float inorm = fisqrt(this->x * this->x + this->y * this->y);
  return Dir2 (
    this->x * inorm,
    this->y * inorm
  );
}
