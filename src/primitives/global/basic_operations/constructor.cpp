#include "../../../../headers/primitives/global.hpp"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <iostream>
#include <cstring>
#include <chrono>

Global::Global (std::string_view window_name, uint32_t height, uint32_t width, SDL_Color bg_color) noexcept {
  /* Initialization of SDL. */
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Could not initialize SDL" << SDL_GetError() << std::endl;
    std::exit(-1);
  }
  
  /* Creation of window. */
  this->window = SDL_CreateWindow (
    window_name.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
    width, height, SDL_WINDOW_SHOWN
  );
  if (this->window == nullptr) {
    std::cerr << "Could not initialize window" << SDL_GetError() << std::endl;
    SDL_Quit ();
    std::exit(-1);
  }

  /* Creation of renderer. */
  this->render = SDL_CreateRenderer (this->window, -1, SDL_RENDERER_ACCELERATED);
  if (this->render == nullptr) {
    std::cerr << "Could not initialize render" << SDL_GetError() << std::endl;
    SDL_DestroyWindow (this->window);
    SDL_Quit ();
    std::exit(-1);
  }

  /* Creation of textures. */
  this->global_render_texture = SDL_CreateTexture(
    this->render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height
  );
  this->render_mask = SDL_CreateTexture(
    this->render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height
  );
  SDL_SetTextureBlendMode(this->render_mask, SDL_BLENDMODE_BLEND);

  /* Initialization of configurations. */
  this->clock_ticks = 0;
  this->c_width = width;
  this->c_height = height;
  this->c_bg_color = bg_color;
  this->c_window_name = window_name;
  this->darkness_color = SDL_Color {0, 0, 0, 0};

  /* Generation of random variables. */
  uint64_t seed = static_cast<uint64_t>(
    std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()
    ).count()
  );
  this->c = 1;
  for (int i = 0; i < 16; i++)
    if (seed & (1 << i))
      this->c *= relatives[i];
  this->a = 2;
  seed = (~seed >> 5) + 0x0000FFFF;
  for (int i = 16; i < 32; i++)
    if (seed & (1 << i))
      this->a *= relatives[i];
  this->a += 1;
  this->xN = 0;
}
