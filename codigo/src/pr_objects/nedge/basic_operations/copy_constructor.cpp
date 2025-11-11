#include "../../../../headers/pr_objects/nedge.hpp"

NEdge::NEdge (const NEdge & poly) noexcept {
  this->points = poly.points;
  this->texture = poly.texture;
}
