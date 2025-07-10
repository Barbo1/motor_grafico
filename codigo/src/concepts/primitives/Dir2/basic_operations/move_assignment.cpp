#include "../../../../../headers/concepts/primitives.hpp"
#include <utility>

Dir2 & Dir2::operator= (Dir2 && dir) {
  this->x = std::exchange(dir.x, 0);
  this->y = std::exchange(dir.y, 0);
  return *this;
}
