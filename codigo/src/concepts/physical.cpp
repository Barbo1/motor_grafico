#pragma once

#include "../../headers/concepts/physical.hpp"

Physical::Physical () {
  this->position = {0, 0, 0};
  this->_density = 0;
  this->_area = 0;
  this->_elasticity = 0;
  this->_f_s = 0;
  this->_f_k = 0;
  this->_movible = true;
  this->_colidalble = false;
  this->_index = 0;
  this->_gcenter = {0, 0};
  this->_force = {0, 0, 0};
  this->_velocity = {0, 0, 0};
}

Physical::Physical (
    AngularDirection position, float density, float area, 
    float elasticity, float f_s, float f_k, bool movible, 
    bool colidable
) {
  this->position = position;
  this->_density = density;
  this->_area = area;
  this->_elasticity = elasticity;
  this->_f_s = f_s;
  this->_f_k = f_k;
  this->_movible = movible;
  this->_colidalble = true;
  this->_index = 0;
  this->_gcenter = position;
  this->_force = {0, 0, 0};
  this->_velocity = {0, 0, 0};
}

void Physical::set_force (const AngularDirection & force) {
  this->_force = force;
}

void Physical::add_force (const AngularDirection & force) {
  this->_force = force;
}

AngularDirection Physical::get_force () const {
  return this->_force;
}

void Physical::set_velocity (const AngularDirection & velocity) {
  this->_velocity = velocity;
}

void Physical::add_velocity (const AngularDirection & velocity) {
  this->_velocity = velocity;
}

AngularDirection Physical::get_velocity () const {
  return this->_velocity;
}
    
void Physical::set_position (AngularDirection pos) {
  this->position = pos;
}

AngularDirection Physical::get_position () const {
  return this->position;
}

void Physical::calculate_movement(std::vector<AngularDirection*> & extrenal_forces) {
  if (this->_movible) {
    for (auto* force: extrenal_forces)
      this->_force += (*force) * (this->_density * this->_area * DRAW_RATE);
    if (this->_movible) {
      this->_velocity += this->_force;
      this->position += this->_velocity * DRAW_RATE;
    }
  }
}
