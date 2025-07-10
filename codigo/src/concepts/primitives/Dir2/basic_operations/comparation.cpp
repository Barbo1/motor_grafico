#include "../../../../../headers/concepts/primitives.hpp"

bool Dir2::operator== (const Dir2 & dir) {
  return this->x == dir.x && this->y == dir.y;
}
    
bool Dir2::operator!= (const Dir2 & dir) {
  return this->x != dir.x || this->y != dir.y;
}
