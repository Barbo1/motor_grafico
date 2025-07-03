#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <cstddef>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <array>

#include "headers/concepts/primitives.hpp"
#include "headers/concepts/texture.hpp"
#include "headers/pr_objects/square.hpp"

const char window_name[] = "Ventana";
const uint32_t height = 800;
const uint32_t width = 600;
const SDL_Color bg_color = {0, 0, 0, 0};

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

  bool step = true;
  bool cont = true;
  AngularDirection gravity = AngularDirection (0, 2, 0);

  std::vector<AngularDirection*> fig_vec = std::vector<AngularDirection*>();
  Square fig (render, 15, 50, {300, 100, 0}, SDL_Color {255, 255, 255, 0});
  fig.set_velocity({0, -1, 2});
  fig.set_force(gravity);
  SDL_Event event;

  std::size_t tope = 70;
  std::size_t min = 5;
  std::array<std::size_t, 3> arr = {tope, tope/2 + min, min};
  int mid = 300;

  while (cont) {
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

    SDL_RenderClear(render);

    for (auto& a: arr) {
      a++;
      if (a >= tope)
        a = min;
      std::size_t b = a;
      float cosi = 1 - static_cast<float>(a) / tope;
      Texture::circunference (render, b, std::roundl(0.3 * std::log2(b)), {
        .r = 255,
        .g = 255,
        .b = 255,
        .a = static_cast<Uint8>(cosi * 255)
      }).draw(render, AngularDirection {
        100 - (float)b, 
        100 - (float)b, 
        0
      });
    }

    if (fig.get_position().y >= (float)mid && step) {
      fig.set_force(-gravity);
      step = false;
    }
    if (fig.get_position().y <= (float)mid && !step) {
      fig.set_force(gravity);
      step = true;
    }

    fig.calculate_movement(fig_vec);
    fig.draw(render);

    SDL_RenderPresent (render);
    SDL_Delay(16);
  } 
  
  SDL_DestroyRenderer (render);
  SDL_DestroyWindow (window);
  SDL_Quit ();
}
