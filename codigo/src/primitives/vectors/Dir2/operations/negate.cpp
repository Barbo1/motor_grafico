#include "../../../../../headers/primitives/vectors.hpp"

Dir2 Dir2::operator- () const & {
  return Dir2(-this->x, -this->y);
}

Dir2 Dir2::operator- () && {
  this->x = -this->x;
  this->y = -this->y;
  return *this;
}
