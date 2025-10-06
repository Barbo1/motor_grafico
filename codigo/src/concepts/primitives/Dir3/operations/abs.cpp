#include "../../../../../headers/concepts/primitives.hpp"
#include "../../../../../headers/concepts/operations.hpp"

Dir3 Dir3::abs () {
  return Dir3 (
    absv (this->x),
    absv (this->y),
    absv (this->z)
  );
}
