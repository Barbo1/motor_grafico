#include "../../../../../headers/concepts/visualizer.hpp"
#include <cstdlib>

Visualizer<D3FIG> Visualizer<D3FIG>::pyramid (SDL_Renderer* render, float height, float base_rad, int base_vert) {
  if (height <= 0 || base_rad <= 0 || base_vert < 3)
    return Visualizer<D3FIG>();

  Visualizer<D3FIG> fig = Visualizer<D3FIG>();
  return fig;
}
