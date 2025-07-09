#include "../../../../../headers/concepts/primitives.hpp"

Direction & Direction::operator= (const Direction & dir) {
  this->x = dir.x;
  this->y = dir.y;
  return *this;
}
