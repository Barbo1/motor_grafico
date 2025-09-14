#include "../../../../../headers/concepts/visualizer.hpp"

template <VisualType T>
Visualizer<T>::~Visualizer () {
  (*this->use_count)--;
  if (*this->use_count <= 0) {
    if (this->texture != nullptr)
      SDL_DestroyTexture (this->texture);
    delete this->use_count;
  }
}

template Visualizer<D2FIG>::~Visualizer ();
