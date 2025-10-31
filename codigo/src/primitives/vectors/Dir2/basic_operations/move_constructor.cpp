#include "../../../../../headers/primitives/vectors.hpp"

Dir2::Dir2 (Dir2 && dir) {
  this->x = dir.x;
  this->y = dir.y;
}
