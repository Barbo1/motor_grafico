#include "../../../../../headers/concepts/primitives.hpp"

float AngDir2::angle () const {
  return (this->x + this->y) / this->modulo();
}
