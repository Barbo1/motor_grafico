#include "../../../../../headers/concepts/visualizer.hpp"

template <VisualType T>
Visualizer<T>::Visualizer (const Visualizer & texture) noexcept {
  this->texture = texture.texture;
  this->width = texture.width;
  this->height = texture.height;
}

template Visualizer<D2FIG>::Visualizer (const Visualizer & texture);
