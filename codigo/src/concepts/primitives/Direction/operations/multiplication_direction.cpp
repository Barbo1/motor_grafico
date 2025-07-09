#include "../../../../../headers/concepts/primitives.hpp"

template<DirFin R> float Direction::operator* (const R & d) {
  return this->x * d.x + this->y * d.y;
}

template float Direction::operator*<AngularDirection>(const AngularDirection&);
template float Direction::operator*<Direction>(const Direction&);
