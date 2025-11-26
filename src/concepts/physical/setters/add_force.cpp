#include "../../../../headers/concepts/physical.hpp"

void Physical::add_force (const AngDir2 & force) {
  this->_force += force;
}
