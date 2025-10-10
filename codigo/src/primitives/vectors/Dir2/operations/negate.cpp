#include "../../../../../headers/primitives/vectors.hpp"

Dir2 Dir2::operator- () {
  return Dir2(-this->x, -this->y);
}
