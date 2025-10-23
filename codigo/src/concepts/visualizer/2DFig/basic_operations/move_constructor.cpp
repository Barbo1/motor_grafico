#include "../../../../../headers/concepts/visualizer.hpp"
#include <utility>

template <VisualType T>
Visualizer<T>::Visualizer (Visualizer && texture) {
  this->texture = std::exchange(texture.texture, nullptr);
  this->height = std::exchange(texture.height, 0);
  this->width = std::exchange(texture.width, 0);
}

template Visualizer<D2FIG>::Visualizer (Visualizer && texture);
