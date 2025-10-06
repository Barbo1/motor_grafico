#include "../../../../../headers/concepts/primitives.hpp"
#include "../../../../../headers/concepts/operations.hpp"

Dir2 Dir2::normalize () {
  float inorm = fisqrt(this->x * this->x + this->y * this->y);
  return Dir2 (
    this->x * inorm,
    this->y * inorm
  );
}
