#include "../../../../../headers/concepts/primitives.hpp"
#include "../../../../../headers/concepts/operations.hpp"

float AngDir2::angle () const {
  return (this->x + this->y) * fisqrt(this->x * this->x + this->y * this->y);
}
