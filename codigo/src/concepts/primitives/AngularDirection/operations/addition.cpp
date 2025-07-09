#include "../../../../../headers/concepts/primitives.hpp"

template<DirFin R> AngularDirection AngularDirection::operator+ (R d) {
  float angle = this->a;
  if constexpr (std::is_same_v<R, AngularDirection>)
    angle += d.a;
  return AngularDirection {
    this->x + d.x,
    this->y + d.y,
    angle
  };
}

template<DirFin R> void AngularDirection::operator+= (R d) {
  this->x += d.x;
  this->y += d.y;
  if constexpr (std::is_same_v<R, AngularDirection>)
    this->a -= d.a;
}

template AngularDirection AngularDirection::operator+<AngularDirection>(AngularDirection);
template void AngularDirection::operator+=<AngularDirection>(AngularDirection);
template AngularDirection AngularDirection::operator+<Direction>(Direction);
template void AngularDirection::operator+=<Direction>(Direction);
