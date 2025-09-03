#include "../../../../../headers/concepts/primitives.hpp"

template<DirFin R> float AngDir2::operator* (R d) const {
  return this->Dir2::operator*(d);
}

template float AngDir2::operator*<Dir2>(Dir2) const;
template float AngDir2::operator*<AngDir2>(AngDir2) const;
