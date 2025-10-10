#include "../../../../../headers/primitives/vectors.hpp"
#include "../../../../../headers/primitives/operations.hpp"

Dir3 Dir3::abs () {
  return Dir3 (
    absv (this->x),
    absv (this->y),
    absv (this->z)
  );
}
