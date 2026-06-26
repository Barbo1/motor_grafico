#include "../../../../headers/concepts/physical.hpp"

void Physical::add_velocity (const AngDir2 & velocity) {
  this->velocity.store(Dir2(velocity) + Dir2(this->velocity));
}
