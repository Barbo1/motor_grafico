#include "../../../../../headers/primitives/vectors.hpp"

AngDir2 AngDir2::percan () const {
  return AngDir2 (
    -this->y,
    this->x,
    0
  );
}
