#include "../../../../../headers/primitives/vectors.hpp"
#include <cmath>

void Dir2::rotate (float angle) {
  float sina = std::sin (angle);
  float cosa = std::cos (angle);
  float newx = this->x * cosa - this->y * sina;
  this->y = this->x * sina + this->y * cosa;
  this->x = newx;
}
