#include "../../../../../headers/concepts/primitives.hpp"

template<DirFin R> Direction Direction::operator+ (const R & d) {
  return Direction (
    this->x + d.x,
    this->y + d.y
  );
}

template<DirFin R> void Direction::operator+= (const R & d) {
  this->x += d.x;
  this->y += d.y;
}

template Direction Direction::operator+<AngularDirection>(const AngularDirection&);
template void Direction::operator+=<AngularDirection>(const AngularDirection&);
template void Direction::operator+=<Direction>(const Direction&);
template Direction Direction::operator+<Direction>(const Direction&);
