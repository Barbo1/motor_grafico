#include "../../../../../headers/concepts/primitives.hpp"
#include <utility>

Direction & Direction::operator= (Direction && dir) {
  this->x = std::exchange(dir.x, 0);
  this->y = std::exchange(dir.y, 0);
  return *this;
}
