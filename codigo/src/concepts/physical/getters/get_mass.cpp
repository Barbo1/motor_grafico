#include "../../../../headers/concepts/physical.hpp"

float Physical::get_mass () const {
  return this->_area * this->_density;
}
