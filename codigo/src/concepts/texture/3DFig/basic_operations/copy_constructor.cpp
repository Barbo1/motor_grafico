#include "../../../../../headers/concepts/texture.hpp"

Visualizer<D3FIG>::Visualizer (const Visualizer & visual) {
  this->texture = visual.texture;
  this->info = visual.info;
  this->use_count = visual.use_count;
  (*this->use_count)++;
}
