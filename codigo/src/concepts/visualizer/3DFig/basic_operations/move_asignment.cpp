#include "../../../../../headers/concepts/visualizer.hpp"
#include <utility>

Visualizer<D3FIG> & Visualizer<D3FIG>::operator= (Visualizer && visual) noexcept {
  this->texture = std::exchange (visual.texture, SDL_Color {255, 255, 255, 255});
  this->info.reset ();
  this->info = std::exchange (visual.info, nullptr);
  return *this;
}
