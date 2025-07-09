#include "../../../../../headers/concepts/primitives.hpp"

Direction Direction::operator* (float f) {
  return Direction (
    this->x * f,
    this->y * f 
  );
}

void Direction::operator*= (float f) {
  this->x *= f;
  this->y *= f;
}
