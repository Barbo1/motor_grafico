#include "../../../headers/pr_objects/square.hpp"

float Square::get_mass () const {
  return this->area * this->density;
}
