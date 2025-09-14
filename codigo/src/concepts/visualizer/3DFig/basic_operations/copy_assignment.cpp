#include "../../../../../headers/concepts/visualizer.hpp"

Visualizer<D3FIG> & Visualizer<D3FIG>::operator= (const Visualizer<D3FIG> & visual) {
  this->texture = visual.texture;
  (*this->use_count)--;
  if ((*this->use_count) <= 0) {
    delete this->info;
    delete this->use_count;
  }
  this->info = visual.info;
  this->use_count = visual.use_count;
  (*this->use_count)++;
  return *this;
}
