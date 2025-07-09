#include "../../../../../headers/concepts/primitives.hpp"

template<DirFin R> float AngularDirection::operator* (R d) {
  return this->Direction::operator*(d);
}

template float AngularDirection::operator*<Direction>(Direction);
template float AngularDirection::operator*<AngularDirection>(AngularDirection);
