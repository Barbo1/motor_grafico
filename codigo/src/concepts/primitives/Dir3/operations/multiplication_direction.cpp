#include "../../../../../headers/concepts/primitives.hpp"

float Dir3::operator* (const Dir3 & d) const {
  return this->x * d.x + this->y * d.y;
}
