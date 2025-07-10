#include "../../../../../headers/concepts/primitives.hpp"

template<DirFin R> float Dir2::operator* (const R & d) {
  return this->x * d.x + this->y * d.y;
}

template float Dir2::operator*<AngDir2>(const AngDir2&);
template float Dir2::operator*<Dir2>(const Dir2&);
