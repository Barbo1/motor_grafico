#include "../../../../../headers/concepts/primitives.hpp"

Dir2 & Dir2::operator= (const Dir2 & dir) {
  this->x = dir.x;
  this->y = dir.y;
  return *this;
}
