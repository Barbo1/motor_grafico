#include "../../../../headers/concepts/physical.hpp"

Physical::Physical (
    AngularDirection position, float density, float area, float elasticity, 
    float f_s, float f_k, bool movible, bool colidable
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

