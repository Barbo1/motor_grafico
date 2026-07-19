#include "../../../headers/pr_objects/circle.hpp"

float Circle::get_mass () const {
  return this->area * this->density;
}

