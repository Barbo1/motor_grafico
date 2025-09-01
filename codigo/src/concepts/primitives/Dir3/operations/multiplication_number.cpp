#include "../../../../../headers/concepts/primitives.hpp"

Dir3 Dir3::operator* (float f) const {
  return Dir3 (
    this->x * f,
    this->y * f,
    this->z * f 
  );
}

void Dir3::operator*= (float f) {
  this->x *= f;
  this->y *= f;
  this->z *= f;
}
