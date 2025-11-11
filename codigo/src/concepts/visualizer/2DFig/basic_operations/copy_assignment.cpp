#include "../../../../../headers/concepts/visualizer.hpp"

template <VisualType T>
Visualizer<T> & Visualizer<T>::operator= (const Visualizer & texture) noexcept {
  this->width = texture.width;
  this->height = texture.height;
  this->texture = texture.texture;
  return *this;
}

template Visualizer<D2FIG> & Visualizer<D2FIG>::operator= (const Visualizer & texture);
