#include "../../../../../headers/primitives/vectors.hpp"
#include "../../../../../headers/primitives/operations.hpp"

Dir2 Dir2::abs () const& {
  return Dir2 (
    absv (this->x),
    absv (this->y)
  );
}

Dir2 Dir2::abs () && {
  absv(&this->x);
  absv(&this->y);
  return *this;
}
