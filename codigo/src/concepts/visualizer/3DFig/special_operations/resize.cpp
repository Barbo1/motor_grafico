#include "../../../../../headers/concepts/visualizer.hpp"

void Visualizer<D3FIG>::resize (float coef) {
  for (auto& vec: this->info->vectors)
    vec *= coef;
}
