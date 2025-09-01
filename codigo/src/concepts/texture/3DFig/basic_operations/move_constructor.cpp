#include "../../../../../headers/concepts/texture.hpp"
#include <utility>

Visualizer<D3FIG>::Visualizer (Visualizer && visual) {
  this->texture = std::exchange(visual.texture, SDL_Color {255, 255, 255, 255});
  this->use_count = std::exchange(visual.use_count, new int(1));
  this->info = std::exchange(visual.info, nullptr);
}
