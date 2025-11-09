#include "../../../../../headers/primitives/vectors.hpp"
#include "../../../../../headers/primitives/operations.hpp"

Dir2 Dir2::normalize () const & {
  float inorm = fisqrt(this->x * this->x + this->y * this->y);
  return Dir2 (
    this->x * inorm,
    this->y * inorm
  );
}

Dir2 Dir2::normalize () && {
  float inorm = fisqrt(this->x * this->x + this->y * this->y);
  this->x *= inorm;
  this->y *= inorm;
  return *this;
}
