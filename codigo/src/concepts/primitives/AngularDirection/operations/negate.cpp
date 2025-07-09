#include "../../../../../headers/concepts/primitives.hpp"

AngularDirection AngularDirection::operator- () {
  return {-this->x, -this->y, this->a};
}
