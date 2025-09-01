#include "../../../../../headers/concepts/texture.hpp"

template <VisualType T>
Visualizer<T>::Visualizer () {
  this->texture = nullptr;
  this->use_count = new int(1);
}

template Visualizer<D2FIG>::Visualizer ();
