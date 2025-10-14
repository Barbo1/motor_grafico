#include "../../../../../headers/concepts/visualizer.hpp"

Visualizer<D3FIG>::Visualizer (const Visualizer & visual) {
  this->texture = visual.texture;
  this->info = visual.info;
}
