#include "../../../../../headers/concepts/primitives.hpp"

AngDir2 AngDir2::operator- () const {
  return {-this->x, -this->y, this->a};
}
