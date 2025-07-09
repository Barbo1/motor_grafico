#include "../../../../../headers/concepts/primitives.hpp"

Direction Direction::operator- () {
  return {-this->x, -this->y};
}
