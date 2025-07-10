#include "../../../../../headers/concepts/primitives.hpp"

Dir3 Dir3::operator- () {
  return {-this->x, -this->y, -this->z};
}
