#include "../../../../../headers/concepts/visualizer.hpp"

Visualizer<D3FIG>::Visualizer (const Visualizer & visual) noexcept {
  this->texture = visual.texture;
  this->info = visual.info;
}
