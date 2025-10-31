#include "../../../../headers/primitives/global.hpp"

Global* Global::single {nullptr};
    
Global* Global::create (std::string_view window_name, uint32_t height, uint32_t width, SDL_Color bg_color) {
  if (Global::single == nullptr)
    Global::single = new Global(window_name, height, width, bg_color);
  return Global::single;
}
