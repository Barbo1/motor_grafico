#include "../../../../../headers/concepts/visualizer.hpp"

Visualizer<D3FIG>::Visualizer () noexcept {
  this->info = std::make_shared<Info>();
  this->texture = SDL_Color { .r = 255, .g = 255, .b = 255, .a = 255 };
}
