#include "../../../../../headers/concepts/visualizer.hpp"

template <VisualType T>
Visualizer<T>::Visualizer (const Visualizer & texture) {
  this->texture = texture.texture;
  this->width = texture.width;
  this->height = texture.height;
  this->use_count = texture.use_count;
  (*texture.use_count)++;
}

template Visualizer<D2FIG>::Visualizer (const Visualizer & texture);
