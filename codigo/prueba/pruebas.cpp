#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

#include "../headers/concepts/primitives.hpp"
#include "../headers/concepts/visualizer.hpp"

const char window_name[] = "Ventana";
const uint32_t height = 800;
const uint32_t width = 600;
const SDL_Color bg_color = {30, 30, 30, 0};

int main () {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Could not initialize SDL" << SDL_GetError() << std::endl;
    return -1;
  }
  
  SDL_Window * window = SDL_CreateWindow (
    window_name, 
    SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED, 
    height, 
    width, 
    SDL_WINDOW_SHOWN
  );
  if (window == nullptr) {
    std::cerr << "Could not initialize window" << SDL_GetError() << std::endl;
    SDL_Quit ();
    return -1;
  }

  SDL_Renderer * render = SDL_CreateRenderer (
    window, 
    -1, 
    SDL_RENDERER_ACCELERATED
  );
  if (render == nullptr) {
    std::cerr << "Could not initialize render" << SDL_GetError() << std::endl;
    SDL_DestroyWindow (window);
    SDL_Quit ();
    return -1;
  }

  bool cont = true;
  SDL_Event event;

  Visualizer<D3FIG> mesita = Visualizer<D3FIG>::prism(render, 1, 1, 1);
  //mesita.set_color(SDL_Color {255, 255, 255, 255});
  mesita.set_texture("../images/rubik.png");
  mesita.resize(60);
  Dir3 cube_pos = Dir3 {300, 300, 300};

  while (cont) {
    SDL_Delay(16);
    SDL_SetRenderDrawColor(render, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    SDL_RenderClear(render);

    mesita.rotate({0.02, 0.01, 0.013});
    mesita.draw(render, cube_pos);
    
    if (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          cont = false;
          break;

        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
              cont = false;
              break;
          }
          break;
      }
    }

    SDL_RenderPresent (render);
  } 
  
  SDL_DestroyRenderer (render);
  SDL_DestroyWindow (window);
  SDL_Quit ();
}
