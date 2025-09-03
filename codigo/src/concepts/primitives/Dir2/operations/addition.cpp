#include "../../../../../headers/concepts/primitives.hpp"

template<DirFin R> Dir2 Dir2::operator+ (const R & d) const {
  return Dir2 (
    this->x + d.x,
    this->y + d.y
  );
}

template<DirFin R> void Dir2::operator+= (const R & d) {
  this->x += d.x;
  this->y += d.y;
}

template Dir2 Dir2::operator+<AngDir2>(const AngDir2&) const;
template void Dir2::operator+=<AngDir2>(const AngDir2&);
template void Dir2::operator+=<Dir2>(const Dir2&);
template Dir2 Dir2::operator+<Dir2>(const Dir2&) const;
