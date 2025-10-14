#include "../../../../../headers/concepts/visualizer.hpp"

Visualizer<D3FIG> & Visualizer<D3FIG>::operator= (const Visualizer<D3FIG> & visual) {
  this->texture = visual.texture;
  this->info.reset();
  this->info = visual.info;
  return *this;
}
