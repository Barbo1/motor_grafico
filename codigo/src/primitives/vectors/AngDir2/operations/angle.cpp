#include "../../../../../headers/primitives/vectors.hpp"
#include "../../../../../headers/primitives/operations.hpp"

float AngDir2::angle () const {
  return (this->x + this->y) * fisqrt(this->x * this->x + this->y * this->y);
}
