#include "../../../../headers/pr_objects/nedge.hpp"

NEdge::NEdge () noexcept {
  this->texture = Visualizer<D2FIG>();
  this->points = std::vector<Dir2>();
}
