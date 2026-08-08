#include "../../../headers/pr_objects/nedge.hpp"

float NEdge::get_mass () const {
  return this->density * this->area;
}
