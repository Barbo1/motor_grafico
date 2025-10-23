#include "../../../../headers/concepts/physical.hpp"

Physical::Physical (
    Global* glb, AngDir2 position, float density, float area, float f_k, bool movible, bool colidable
) {
  this->position = position;
  this->_density = density;
  this->_area = area;
  this->_f_k = f_k;
  this->_movible = movible;
  this->_colidalble = colidable;
  this->_normal_presence = false;
  this->_force = {0, 0, 0};
  this->_velocity = {0, 0, 0};
  this->glb = glb;
}
