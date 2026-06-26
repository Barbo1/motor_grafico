#include "../../../../headers/concepts/physical.hpp"
    
void Physical::set_position (const AngDir2& pos) {
  this->position.store(pos);
}
