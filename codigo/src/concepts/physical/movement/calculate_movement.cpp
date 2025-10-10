#include "../../../../headers/concepts/physical.hpp"

void Physical::calculate_movement(const AngDir2 & extrenal_forces) {
  if (this->_movible) {
    AngDir2 f = this->_force + extrenal_forces;
    f *= MOVEMENT_COEFITIENT / (this->_density * this->_area);
    this->_velocity += f;
    this->position += this->_velocity * DRAW_RATE;
  }
}
