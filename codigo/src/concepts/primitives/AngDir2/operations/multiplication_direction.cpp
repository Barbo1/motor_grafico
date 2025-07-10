#include "../../../../../headers/concepts/primitives.hpp"

template<DirFin R> float AngDir2::operator* (R d) {
  return this->Dir2::operator*(d);
}

template float AngDir2::operator*<Dir2>(Dir2);
template float AngDir2::operator*<AngDir2>(AngDir2);
