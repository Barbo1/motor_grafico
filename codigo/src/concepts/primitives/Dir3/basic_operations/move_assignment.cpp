#include "../../../../../headers/concepts/primitives.hpp"
#include <utility>

Dir3 & Dir3::operator= (Dir3 && dir) {
  this->x = std::exchange(dir.x, 0);
  this->y = std::exchange(dir.y, 0);
  this->z = std::exchange(dir.z, 0);
  return *this;
}
