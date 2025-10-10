#include "../../../../../headers/primitives/vectors.hpp"
#include <utility>

Dir2::Dir2 (Dir2 && dir) {
  this->x = std::exchange(dir.x, 0);
  this->y = std::exchange(dir.y, 0);
}
