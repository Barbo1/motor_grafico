#include "../../../../../headers/concepts/visualizer.hpp"
#include <utility>

template <VisualType T>
Visualizer<T> & Visualizer<T>::operator= (Visualizer && texture) noexcept {
  this->height = std::exchange(texture.height, 0);
  this->width = std::exchange(texture.width, 0);
  this->texture = std::exchange(texture.texture, nullptr);
  return *this;
}

template Visualizer<D2FIG> & Visualizer<D2FIG>::operator= (Visualizer && texture);
