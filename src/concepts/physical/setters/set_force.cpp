#include "../../../../headers/concepts/physical.hpp"

void Physical::set_force (const AngDir2 & force) {
  this->force.store(force);
}
