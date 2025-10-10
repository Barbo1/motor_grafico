#include "../../../../../headers/primitives/vectors.hpp"
#include <cmath>

void Dir3::rotate_x (float angle) {
  float sina = std::sin (angle);
  float cosa = std::cos (angle);
  float newy = this->z * sina + this->y * cosa;
  this->z = this->z * cosa - this->y * sina;
  this->y = newy;
}

void Dir3::rotate_y (float angle) {
  float sina = std::sin (angle);
  float cosa = std::cos (angle);
  float newx = this->x * cosa - this->z * sina;
  this->z = this->z * cosa + this->x * sina;
  this->x = newx;
}

void Dir3::rotate_z (float angle) {
  float sina = std::sin (angle);
  float cosa = std::cos (angle);
  float newx = this->x * cosa + this->y * sina;
  this->y = this->y * cosa - this->x * sina;
  this->x = newx;
}
