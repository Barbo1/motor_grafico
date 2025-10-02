#include "../../../../headers/concepts/visualizer.hpp"

Visualizer<D2FIG> chargePNG (SDL_Renderer* render, const std::string& path) {
  Visualizer<D2FIG> tex;
  int w, h;
  Uint32 * pixels = charging_PNG_to_memory(path, w, h);
  if (pixels != nullptr) {
    tex = Visualizer<D2FIG> (render, h, w, pixels);
    delete [] pixels;
  } else tex = Visualizer<D2FIG> ();
  return tex;
}
