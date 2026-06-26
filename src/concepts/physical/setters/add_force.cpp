#include "../../../../headers/concepts/physical.hpp"

void Physical::add_force (const AngDir2 & force) {
  this->force.store(Dir2(force) + Dir2(this->force));
}
