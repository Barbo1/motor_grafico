#include "../../../../headers/concepts/physical.hpp"

void Physical::calculate_movement(const std::vector<AngDir2*> & extrenal_forces) {
  if (this->_movible) {
    for (auto* force: extrenal_forces)
      this->_force += (*force) * (this->_density * this->_area * DRAW_RATE);
    if (this->_movible) {
      this->_velocity += this->_force;
      this->position += this->_velocity * DRAW_RATE;
    }
  }
}
