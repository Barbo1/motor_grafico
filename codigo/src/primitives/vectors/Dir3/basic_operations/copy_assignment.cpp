#include "../../../../../headers/primitives/vectors.hpp"

Dir3 & Dir3::operator= (const Dir3 & dir) {
  this->x = dir.x;
  this->y = dir.y;
  this->z = dir.z;
  return *this;
}
