#include "../../../../headers/concepts/global.hpp"

Global* Global::single {nullptr};
    
Global* Global::create (std::string_view window_name, SDL_Color bg_color) noexcept {
  if (Global::single == nullptr)
    Global::single = new Global(window_name, bg_color);
  return Global::single;
}
