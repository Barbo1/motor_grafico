#include "../../../../../headers/concepts/primitives.hpp"

bool Direction::operator== (const Direction & dir) {
  return this->x == dir.x && this->y == dir.y;
}
