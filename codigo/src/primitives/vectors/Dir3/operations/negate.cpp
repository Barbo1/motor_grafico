#include "../../../../../headers/primitives/vectors.hpp"

Dir3 Dir3::operator- () const {
  return {-this->x, -this->y, -this->z};
}
