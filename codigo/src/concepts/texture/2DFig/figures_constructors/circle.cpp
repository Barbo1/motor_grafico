#include "../../../../../headers/concepts/texture.hpp"
#include "../utils/circle_bound_maker.cpp"

template <VisualType T>
Visualizer<T> Visualizer<T>::circle (SDL_Renderer* render, int radio, SDL_Color color) {
  int* bounds = new int[4*radio - 4];
  circle_bound_maker (radio, bounds, 0);

  Visualizer<T> ret = bounder(render, bounds, 2*radio, 2*radio, color);
  delete [] bounds;
  return ret;
}

template Visualizer<D2FIG> Visualizer<D2FIG>::circle (SDL_Renderer* render, int radio, SDL_Color color);
