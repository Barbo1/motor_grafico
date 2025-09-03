#include "../../../../../headers/concepts/texture.hpp"

template <VisualType T>
Visualizer<T> & Visualizer<T>::operator= (const Visualizer & texture) {
  this->width = texture.width;
  this->height = texture.height;
  (*this->use_count)--;
  if ((*this->use_count) <= 0) {
    if (this->texture != nullptr)
      SDL_DestroyTexture (this->texture);
    delete this->use_count;
  }
  this->texture = texture.texture;
  this->use_count = texture.use_count;
  (*this->use_count)++;
  return *this;
}

template Visualizer<D2FIG> & Visualizer<D2FIG>::operator= (const Visualizer & texture);
