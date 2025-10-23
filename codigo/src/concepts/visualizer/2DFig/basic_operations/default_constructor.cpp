#include "../../../../../headers/concepts/visualizer.hpp"

template <VisualType T>
Visualizer<T>::Visualizer () {
  this->texture = nullptr;
}

template Visualizer<D2FIG>::Visualizer ();
