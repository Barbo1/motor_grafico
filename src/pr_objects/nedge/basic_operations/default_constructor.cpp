#include "../../../../headers/pr_objects/nedge.hpp"

template<std::size_t N>
NEdge<N>::NEdge () noexcept {
  this->texture = Visualizer<D2FIG>();
  this->points = std::vector<Dir2>();
}
