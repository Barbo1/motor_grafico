#include "../../../../../headers/concepts/primitives.hpp"
#include <cstdlib>

Dir2 Dir2::abs () {
  return Dir2 (
    std::abs(this->x),
    std::abs(this->y)
  );
}
