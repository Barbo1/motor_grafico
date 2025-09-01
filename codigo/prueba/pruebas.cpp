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

#include "../headers/concepts/primitives.hpp"
#include "../headers/concepts/texture.hpp"
#include "../headers/pr_objects/square.hpp"

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

  bool step = true;
  bool cont = true;
  SDL_Event event;

  AngDir2 gravity = AngDir2 (0, 2, 0);
  std::vector<AngDir2*> fig_vec = std::vector<AngDir2*>();
  Square fig (render, 15, 50, {300, 100, 0}, SDL_Color {255, 255, 255, 255});
  fig.set_velocity({0, -1, 2});
  fig.set_force(gravity);

  Dir3 A = {50, 50, 50};
  Dir3 B = {-50, 50, 50};
  Dir3 C = {-50, -50, 50};
  Dir3 D = {50, -50, 50};
  Dir3 E = {50, 50, -50};
  Dir3 F = {-50, 50, -50};
  Dir3 G = {-50, -50, -50};
  Dir3 H = {50, -50, -50};

  std::vector<std::vector<Dir3>> points = {
    {A, B, C}, {A, C, D}, {E, F, G}, {E, G, H},
    {A, B, F}, {A, F, E}, {D, C, G}, {D, G, H},
    {A, E, H}, {A, H, D}, {B, F, G}, {B, G, C}
  };
  std::vector<Dir3> normals = {
    {0,0,1}, {0,0,1}, {0,0,-1}, {0,0,-1},
    {0,1,0}, {0,1,0}, {0,-1,0}, {0,-1,0},
    {1,0,0}, {1,0,0}, {-1,0,0}, {-1,0,0}
  };
  Visualizer<D3FIG> cube = Visualizer<D3FIG> (points, normals);
  cube.set_color (SDL_Color{.r=255, .g=0, .b=0, .a=255});
  Dir3 cube_pos = Dir3 {200, 200, 200};

  std::size_t tope = 70;
  std::size_t min = 5;
  std::array<float, 3> arr = {static_cast<float>(tope), static_cast<float>(tope/2 + min), static_cast<float>(min)};
  std::vector<Visualizer<D2FIG>> textures;

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

    SDL_SetRenderDrawColor(render, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    SDL_RenderClear(render);

    for (auto& a: arr) {
      a+=0.5;
      if (a >= tope)
        a = min;
      std::size_t b = a;
      float cosi = 1 - a / tope;
      Visualizer<D2FIG>::circunference (render, b, std::roundl(0.3 * std::log2(b)), {
        .r = 255,
        .g = 255,
        .b = 255,
        .a = static_cast<Uint8>(cosi * 255)
      }).draw(render, AngDir2 {
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

    cube.rotate({0.02, 0.01, 0.013});
    cube.draw(render, cube_pos);

    fig.calculate_movement(fig_vec);
    fig.draw(render);

    print_triangle_c (
      render, 
      Dir2 {400, 403}, 
      Dir2 {500, 450}, 
      Dir2 {400, 450}, 
      SDL_Color {.r = 255, .g = 255, .b = 255, .a = 255}
    );
    print_triangle_c (
      render, 
      Dir2 {450, 453}, 
      Dir2 {500, 450}, 
      Dir2 {400, 450}, 
      SDL_Color {.r = 255, .g = 255, .b = 255, .a = 255}
    );

    float posy = 0;
    float posx = 300;
    for (auto& tex: textures) {
      tex.draw (render, AngDir2 (posx, posy, 0));
      posy += 50;
      if (posy == 600) {
        posy = 0;
        posx += 50;
      }
    }

    SDL_RenderPresent (render);
    SDL_Delay(16);
  } 
  
  SDL_DestroyRenderer (render);
  SDL_DestroyWindow (window);
  SDL_Quit ();
}
