#include "../../../../../headers/primitives/vectors.hpp"

Dir2 Dir2::percan () const {
  return Dir2 (
    -this->y,
    this->x
  );
}
