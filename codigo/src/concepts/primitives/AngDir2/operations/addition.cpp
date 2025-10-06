#include "../../../../../headers/concepts/primitives.hpp"

template<DirFin R> AngDir2 AngDir2::operator+ (R d) const {
  float angle = this->a;
  if constexpr (std::is_same_v<R, AngDir2>)
    angle += d.a;
  return AngDir2 (
    this->x + d.x,
    this->y + d.y,
    angle
  );
}

template<DirFin R> void AngDir2::operator+= (R d) {
  this->x += d.x;
  this->y += d.y;
  if constexpr (std::is_same_v<R, AngDir2>)
    this->a -= d.a;
}

template AngDir2 AngDir2::operator+<AngDir2>(AngDir2) const;
template void AngDir2::operator+=<AngDir2>(AngDir2);
template AngDir2 AngDir2::operator+<Dir2>(Dir2) const;
template void AngDir2::operator+=<Dir2>(Dir2);
