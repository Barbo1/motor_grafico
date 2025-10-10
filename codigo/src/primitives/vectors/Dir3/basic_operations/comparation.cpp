#include "../../../../../headers/primitives/vectors.hpp"

bool Dir3::operator== (const Dir3 & dir) {
  return this->x == dir.x && this->y == dir.y && this->z == dir.z;
}

bool Dir3::operator!= (const Dir3 & dir) {
  return this->x != dir.x || this->y != dir.y || this->z != dir.z;
}
