#include "../../../../headers/primitives/global.hpp"
#include <cstdint>
#include <iostream>
#include <cstring>

Global::Global (std::string_view window_name, uint32_t height, uint32_t width, SDL_Color bg_color) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Could not initialize SDL" << SDL_GetError() << std::endl;
    std::exit(-1);
  }
  
  this->window = SDL_CreateWindow (
    window_name.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
    width, height, SDL_WINDOW_SHOWN
  );
  if (this->window == nullptr) {
    std::cerr << "Could not initialize window" << SDL_GetError() << std::endl;
    SDL_Quit ();
    std::exit(-1);
  }

  this->render = SDL_CreateRenderer (this->window, -1, SDL_RENDERER_ACCELERATED);
  if (this->render == nullptr) {
    std::cerr << "Could not initialize render" << SDL_GetError() << std::endl;
    SDL_DestroyWindow (this->window);
    SDL_Quit ();
    std::exit(-1);
  }

  this->global_render_texture = SDL_CreateTexture(
    this->render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height
  );

  this->clock_ticks = 0;
  this->c_width = width;
  this->c_height = height;
  this->c_bg_color = bg_color;
  this->c_window_name = window_name;
}
