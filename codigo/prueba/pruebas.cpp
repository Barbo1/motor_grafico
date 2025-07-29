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
#include "../headers/pr_objects/fig3D.hpp"

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
  AngDir2 gravity = AngDir2 (0, 2, 0);

  std::vector<AngDir2*> fig_vec = std::vector<AngDir2*>();
  Square fig (render, 15, 50, {300, 100, 0}, SDL_Color {255, 255, 255, 0});
  fig.set_velocity({0, -1, 2});
  fig.set_force(gravity);
  SDL_Event event;

  Dir3 B = {-50, 50, 50};
  Dir3 F = {-50, 50, -50};
  Dir3 G = {-50, -50, -50};
  Dir3 C = {-50, -50, 50};
  Dir3 A = {50, 50, 50};
  Dir3 E = {50, 50, -50};
  Dir3 H = {50, -50, -50};
  Dir3 D = {50, -50, 50};
  std::vector<std::vector<Dir3>> points = {
    {A, B, C, D},
    {E, F, G, H},
    {A, B, F, E},
    {D, C, G, H},
    {B, F, G, C},
    {A, E, H, D}
  };
  fig3D cube = fig3D (points);
  cube.set_color (std::vector<SDL_Color>{
    {.r=255, .g=0, .b=0, .a=255},
    {.r=0, .g=255, .b=0, .a=255},
    {.r=0,  .g=0, .b=255, .a=255},
    {.r=255,  .g=255, .b=0, .a=255},
    {.r=0,  .g=255, .b=255, .a=255},
    {.r=255,  .g=0, .b=255, .a=255},
  });
  cube.set_position({200, 200, 200});

  std::size_t tope = 70;
  std::size_t min = 5;
  std::array<float, 3> arr = {static_cast<float>(tope), static_cast<float>(tope/2 + min), static_cast<float>(min)};
  std::vector<Texture> textures;

  textures.push_back (chargePNG (render, "../images/pngs/basn0g01.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn0g02.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn0g04.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn0g08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn0g16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn2c08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn2c16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn3p01.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn3p02.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn3p04.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn3p08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn4a08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn4a16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn6a08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basn6a16.png"));
  textures.push_back (Texture());

  textures.push_back (chargePNG (render, "../images/pngs/tbbn0g04.png"));
  textures.push_back (chargePNG (render, "../images/pngs/tbbn2c16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/tbbn3p08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/tbgn2c16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/tbgn3p08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/tbrn2c08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/tbwn0g16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/tbwn3p08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/tbyn3p08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/tp0n0g08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/tp0n2c08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/tp0n3p08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/tp1n3p08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/tm3n3p02.png"));
  textures.push_back (Texture());

  textures.push_back (chargePNG (render, "../images/pngs/f00n0g08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/f00n2c08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/f01n0g08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/f01n2c08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/f02n0g08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/f02n2c08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/f03n0g08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/f04n0g08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/f04n2c08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/f99n0g04.png"));
  textures.push_back (Texture());

  textures.push_back (chargePNG (render, "../images/pngs/pp0n2c16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/pp0n6a08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/ps1n0g08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/ps1n2c16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/ps2n0g08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/ps2n2c16.png"));
  textures.push_back (Texture());
  
  textures.push_back (chargePNG (render, "../images/pngs/oi9n2c16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/oi9n0g16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/oi4n2c16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/oi4n0g16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/oi2n2c16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/oi1n2c16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/oi1n0g16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/oi2n0g16.png"));
  textures.push_back (Texture());
  
  textures.push_back (chargePNG (render, "../images/pngs/z00n2c08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/z03n2c08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/z06n2c08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/z09n2c08.png"));
  textures.push_back (Texture());
 
  textures.push_back (chargePNG (render, "../images/pngs/basi0g01.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi0g02.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi0g04.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi0g08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi0g16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi2c08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi2c16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi3p01.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi3p02.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi3p04.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi3p08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi4a08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi4a16.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi6a08.png"));
  textures.push_back (chargePNG (render, "../images/pngs/basi6a16.png"));
  textures.push_back (Texture());

  /*
  std::cout << "muestra de corrompidos" << std::endl;
  std::cout << "1 ->" << std::endl;
  textures.push_back (chargePNG (render, "../images/pngs/xs1n0g01.png"));
  std::cout << "2 ->" << std::endl;
  textures.push_back (chargePNG (render, "../images/pngs/xs2n0g01.png"));
  std::cout << "3 ->" << std::endl;
  textures.push_back (chargePNG (render, "../images/pngs/xs4n0g01.png"));
  std::cout << "4 ->" << std::endl;
  textures.push_back (chargePNG (render, "../images/pngs/xs7n0g01.png"));
  std::cout << "5 ->" << std::endl;
  textures.push_back (chargePNG (render, "../images/pngs/xcrn0g04.png"));
  std::cout << "6 ->" << std::endl;
  textures.push_back (chargePNG (render, "../images/pngs/xlfn1g04.png"));
  //std::cout << "7 ->" << std::endl;
  //textures.push_back (chargePNG (render, "../images/pngs/xhdn0g08.png"));
  std::cout << "8 ->" << std::endl;
  textures.push_back (chargePNG (render, "../images/pngs/xc1n0g08.png"));
  std::cout << "9 ->" << std::endl;
  textures.push_back (chargePNG (render, "../images/pngs/xc9n2c08.png"));
  std::cout << "10 ->" << std::endl;
  textures.push_back (chargePNG (render, "../images/pngs/xd0n2c08.png"));
  std::cout << "11 ->" << std::endl;
  textures.push_back (chargePNG (render, "../images/pngs/xd3n2c08.png"));
  std::cout << "12 ->" << std::endl;
  textures.push_back (chargePNG (render, "../images/pngs/xd9n2c08.png"));
  std::cout << "13 ->" << std::endl;
  textures.push_back (chargePNG (render, "../images/pngs/xdtn0g01.png"));
  //std::cout << "14 ->" << std::endl;
  //textures.push_back (chargePNG (render, "../images/pngs/xcsn0g01.png"));
  */
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
      Texture::circunference (render, b, std::roundl(0.3 * std::log2(b)), {
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
    cube.draw(render);

    fig.calculate_movement(fig_vec);
    fig.draw(render);

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
