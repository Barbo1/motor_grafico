#include "../../../../../headers/concepts/primitives.hpp"

Dir2 Dir2::operator- () {
  return {-this->x, -this->y};
}
