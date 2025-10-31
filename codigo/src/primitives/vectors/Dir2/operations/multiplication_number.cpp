#include "../../../../../headers/primitives/vectors.hpp"

Dir2 Dir2::operator* (float f) const & {
  return Dir2 (
    this->x * f,
    this->y * f 
  );
}

Dir2 Dir2::operator* (float f) && {
  this->x *= f;
  this->y *= f;
  return *this;
}

void Dir2::operator*= (float f) {
  this->x *= f;
  this->y *= f;
}
