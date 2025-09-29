#include "../../../../headers/pr_objects/nedge.hpp"

NEdge & NEdge::operator=  (const NEdge & poly) {
  this->points = poly.points;
  this->texture = poly.texture;
  return *this;
}
