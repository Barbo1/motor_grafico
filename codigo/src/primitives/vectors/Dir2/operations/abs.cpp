#include "../../../../../headers/primitives/vectors.hpp"
#include "../../../../../headers/primitives/operations.hpp"

Dir2 Dir2::abs () {
  return Dir2 (
    absv (this->x),
    absv (this->y)
  );
}
