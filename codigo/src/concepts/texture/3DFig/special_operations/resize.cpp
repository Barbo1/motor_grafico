#include "../../../../../headers/concepts/texture.hpp"

void Visualizer<D3FIG>::resize (float coef) {
  for (auto& vec: this->info->vectors)
    vec *= coef;
}
