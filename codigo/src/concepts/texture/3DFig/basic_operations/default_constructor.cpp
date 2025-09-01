#include "../../../../../headers/concepts/texture.hpp"
#include <vector>

Visualizer<D3FIG>::Visualizer () {
  this->info = new Visualizer<D3FIG>::Info {
    .vectors = std::vector<Dir3>(),
    .normals = std::vector<Dir3>(),
    .mapping = std::vector<Dir2>(),
    .indeces = std::vector<Face>()
  };
  this->use_count = new int(1);
  this->texture = SDL_Color {
    .r = 255,
    .g = 255,
    .b = 255,
    .a = 255,
  };
}
