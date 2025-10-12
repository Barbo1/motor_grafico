#include "../../../../headers/concepts/physical.hpp"

Physical::Physical () {
  this->position = {0, 0, 0};
  this->_density = 0;
  this->_area = 0;
  this->_f_k = 0;
  this->_movible = true;
  this->_colidalble = false;
  this->_normal_presence = false;
  this->_force = {0, 0, 0};
  this->_velocity = {0, 0, 0};
}
