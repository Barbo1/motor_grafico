#include "../../../../headers/pr_objects/nedge.hpp"
#include <utility>

NEdge & NEdge::operator= (NEdge && poly) noexcept {
  this->texture = std::exchange (poly.texture, Visualizer<D2FIG>());
  this->points = std::exchange (poly.points, std::vector<Dir2>());
  return *this;
}
