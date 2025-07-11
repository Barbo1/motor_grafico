#include "../../../../../headers/concepts/primitives.hpp"

Dir3 Dir3::operator- (const Dir3 & d) {
  return Dir3 (
    this->x - d.x,
    this->y - d.y,
    this->z - d.z
  );
}

void Dir3::operator-= (const Dir3 & d) {
  this->x -= d.x;
  this->y -= d.y;
  this->z -= d.z;
}
