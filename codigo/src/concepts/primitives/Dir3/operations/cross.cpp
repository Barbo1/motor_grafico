#include "../../../../../headers/concepts/primitives.hpp"

Dir3 Dir3::cross (const Dir3& vec) const {
  return Dir3 (
    this->y*vec.z - this->z*vec.y,
    this->z*vec.x - this->x*vec.z,
    this->x*vec.y - this->y*vec.x
  );
}
