#include "../../../../../headers/concepts/texture.hpp"
#include <utility>

template <VisualType T>
Visualizer<T> & Visualizer<T>::operator= (Visualizer && texture) {
  this->height = std::exchange(texture.height, 0);
  this->width = std::exchange(texture.width, 0);
  (*this->use_count)--;
  if ((*this->use_count) <= 0) {
    if (this->texture != nullptr)
      SDL_DestroyTexture (this->texture);
    delete this->use_count;
  }
  this->use_count = std::exchange(texture.use_count, new int(1));
  this->texture = std::exchange(texture.texture, nullptr);
  return *this;
}

template Visualizer<D2FIG> & Visualizer<D2FIG>::operator= (Visualizer && texture);
