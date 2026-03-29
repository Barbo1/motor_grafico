#include "../../../../headers/pr_objects/nedge.hpp"

template<std::size_t N>
Visualizer<D2FIG> NEdge<N>::get_texture () const {
  return this->texture;
}
