#include "../../../../headers/pr_objects/nedge.hpp"

NEdge::NEdge (const NEdge & poly) {
  this->points = poly.points;
  this->texture = poly.texture;
}
