#include "../../headers/concepts/lights.hpp"
#include "../../headers/concepts/image_modifier.hpp"
#include "../../headers/primitives/global.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <iostream>

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 786, 1280, SDL_Color {30, 30, 30, 0});
  float aux_time_1 = 0.f, avg_time_1 = 0.f;
  bool cont = true;
  SDL_Event event;
 
  std::vector<Dir2> poligon_1 = {
    Dir2 {94.6f, 150.0f}, Dir2 {132.6f, 285.3f}, Dir2 {291.4f, 177.9f},  
    Dir2 {256.8f, 63.8f}, Dir2 {163.9f, 59.4f}
  };
 
  std::vector<Dir2> poligon_2 = {
    Dir2 {554.3f, 352.4f}, Dir2 {504.0f, 86.2f}, Dir2 {670.7f, 193.6f},
    Dir2 {677.7f, 70.5}, Dir2 {771.7, 228.3f}, Dir2 {683.0f, 378.2f},
  };
 
  std::vector<Dir2> poligon_3 = {
    Dir2 {28.4f, 501.7f}, Dir2 {100.0f, 400.0f}, Dir2 {400.0f, 400.0f},
    Dir2 {284.3f, 441.9f}, Dir2 {478.2f, 452.6f}, Dir2 {432.6f, 566.3f},
    Dir2 {226.3f, 485.2f}, Dir2 {192.0f, 578.7f}
  };
 
  std::vector<Dir2> poligon_4 = {
    Dir2 {300.f, 300.f}, Dir2 {400.f, 300.f}, Dir2 {450.0f, 350.0f},
    Dir2 {394.1f, 228.7f}, Dir2 {277.0f, 248.9f}
  };

  std::vector<Dir2> curr = poligon_4;
  uint32_t many = curr.size ();
  Dir2 mid = Dir2 {};
  for (uint32_t i = 0; i < many; i++) {
    mid += curr[i];
  }
  mid *= 1.f / static_cast<float>(many);

  auto rotate = [&] () {
    for (Dir2& dir: curr) {
      Dir2 aux = dir - mid;
      aux.rotate(0.01);
      dir = aux + mid;
    }
  };

  while (cont) {

    glb->begin_render();
      aux_time_1 += 1.f;
      
      print_polygon_c (glb, curr, SDL_Color {0, 255, 0, 255});

      float a = glb->get_time();
      avg_time_1 += (a - avg_time_1) / aux_time_1;
      std::cout << ", tiempo: " << a << ", avg: " << avg_time_1 << std::endl;
    glb->end_render();
    
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
            case SDLK_SPACE:
              rotate ();
              break;
          }
          break;
      }
    }

    uint32_t delta = glb->get_ticks();
    if (delta < SCREEN_TICKS_PER_FRAME) {
      SDL_Delay (SCREEN_TICKS_PER_FRAME - delta);
    }
  } 
}
