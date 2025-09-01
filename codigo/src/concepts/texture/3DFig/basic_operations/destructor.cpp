#include "../../../../../headers/concepts/texture.hpp"

Visualizer<D3FIG>::~Visualizer () {
  (*this->use_count)--;
  if (*this->use_count <= 0) {
    delete this->info;
    delete this->use_count;
  }
}

