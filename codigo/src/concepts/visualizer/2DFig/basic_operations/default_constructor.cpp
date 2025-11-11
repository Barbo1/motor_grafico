#include "../../../../../headers/concepts/visualizer.hpp"

template <VisualType T>
Visualizer<T>::Visualizer () noexcept {
  this->texture = nullptr;
}

template Visualizer<D2FIG>::Visualizer ();
