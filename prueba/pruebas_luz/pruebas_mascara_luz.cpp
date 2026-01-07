#include "../headers/concepts/lights.hpp"
#include "../headers/primitives/global.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <iostream>

std::vector<MaskObject> get_segments_1 () {
  return {
    MaskObject {.point1 = Dir2 {134.f, 114.f}, .point2 = Dir2 {52.f, 274.f}, .circle = false},
    MaskObject {.point1 = Dir2 {245.f, 103.f}, .point2 = Dir2 {604.f, 76.f}, .circle = false},
    MaskObject {.point1 = Dir2 {678.f, 149.f}, .point2 = Dir2 {411.f, 178.f}, .circle = false},
    MaskObject {.point1 = Dir2 {117.f, 355.f}, .point2 = Dir2 {278.f, 195.f}, .circle = false},
    MaskObject {.point1 = Dir2 {531.f, 218.f}, .point2 = Dir2 {634.f, 210.f}, .circle = false},
    MaskObject {.point1 = Dir2 {566.f, 441.f}, .point2 = Dir2 {382.f, 313.f}, .circle = false},
    MaskObject {.point1 = Dir2 {617.f, 466.f}, .point2 = Dir2 {388.f, 437.f}, .circle = false},
    MaskObject {.point1 = Dir2 {121.f, 265.f}, .point2 = Dir2 {172.f, 199.f}, .circle = false},
    MaskObject {.point1 = Dir2 {464.f, 348.f}, .point2 = Dir2 {447.f, 337.f}, .circle = false},
    MaskObject {.point1 = Dir2 {498.f, 370.f}, .point2 = Dir2 {506.f, 377.f}, .circle = false},
    MaskObject {.point1 = Dir2 {529.f, 390.f}, .point2 = Dir2 {546.f, 407.f}, .circle = false},
    MaskObject {.point1 = Dir2 {540.f, 320.f}, .point2 = Dir2 {595.f, 355.f}, .circle = false},
  };
}

std::vector<MaskObject> get_segments_2 () {
  return {
    MaskObject {.point1 = Dir2 {122.f, 84.f}, .point2 = Dir2 {86.f, 125.f}, .circle = false},
    MaskObject {.point1 = Dir2 {86.f, 125.f}, .point2 = Dir2 {53.f, 1.78f}, .circle = false},
    MaskObject {.point1 = Dir2 {53.f, 1.78f}, .point2 = Dir2 {62.f, 221.f}, .circle = false},
    MaskObject {.point1 = Dir2 {62.f, 221.f}, .point2 = Dir2 {121.f, 196.f}, .circle = false},
    MaskObject {.point1 = Dir2 {121.f, 196.f}, .point2 = Dir2 {112.f, 243.f}, .circle = false},
    MaskObject {.point1 = Dir2 {112.f, 243.f}, .point2 = Dir2 {20.f, 302.f}, .circle = false},
    MaskObject {.point1 = Dir2 {20.f, 302.f}, .point2 = Dir2 {50.f, 360.f}, .circle = false},
    MaskObject {.point1 = Dir2 {50.f, 360.f}, .point2 = Dir2 {106.f, 330.f}, .circle = false},
    MaskObject {.point1 = Dir2 {106.f, 330.f}, .point2 = Dir2 {60.f, 315.f}, .circle = false},
    MaskObject {.point1 = Dir2 {60.f, 315.f}, .point2 = Dir2 {108.f, 293.f}, .circle = false},
    MaskObject {.point1 = Dir2 {108.f, 293.f}, .point2 = Dir2 {164.f, 311.f}, .circle = false},
    MaskObject {.point1 = Dir2 {164.f, 311.f}, .point2 = Dir2 {110.f, 378.f}, .circle = false},
    MaskObject {.point1 = Dir2 {110.f, 378.f}, .point2 = Dir2 {40.f, 429.f}, .circle = false},
    MaskObject {.point1 = Dir2 {40.f, 429.f}, .point2 = Dir2 {37.f, 470.f}, .circle = false},
    MaskObject {.point1 = Dir2 {37.f, 470.f}, .point2 = Dir2 {127.f, 502.f}, .circle = false},
    MaskObject {.point1 = Dir2 {127.f, 502.f}, .point2 = Dir2 {330.f, 468.f}, .circle = false},
    MaskObject {.point1 = Dir2 {330.f, 468.f}, .point2 = Dir2 {157.f, 438.f}, .circle = false},
    MaskObject {.point1 = Dir2 {157.f, 438.f}, .point2 = Dir2 {282.f, 357.f}, .circle = false},
    MaskObject {.point1 = Dir2 {282.f, 357.f}, .point2 = Dir2 {546.f, 482.f}, .circle = false},
    MaskObject {.point1 = Dir2 {546.f, 482.f}, .point2 = Dir2 {765.f, 473.f}, .circle = false},
    MaskObject {.point1 = Dir2 {765.f, 473.f}, .point2 = Dir2 {750.f, 245.f}, .circle = false},
    MaskObject {.point1 = Dir2 {750.f, 245.f}, .point2 = Dir2 {703.f, 284.f}, .circle = false},
    MaskObject {.point1 = Dir2 {703.f, 284.f}, .point2 = Dir2 {500.f, 200.f}, .circle = false},
    MaskObject {.point1 = Dir2 {500.f, 200.f}, .point2 = Dir2 {532.f, 287.f}, .circle = false},
    MaskObject {.point1 = Dir2 {532.f, 287.f}, .point2 = Dir2 {409.f, 230.f}, .circle = false},
    MaskObject {.point1 = Dir2 {409.f, 230.f}, .point2 = Dir2 {446.f, 104.f}, .circle = false},
    MaskObject {.point1 = Dir2 {446.f, 104.f}, .point2 = Dir2 {269.f, 139.f}, .circle = false},
    MaskObject {.point1 = Dir2 {269.f, 139.f}, .point2 = Dir2 {250.f, 117.f}, .circle = false},
    MaskObject {.point1 = Dir2 {250.f, 117.f}, .point2 = Dir2 {239.f, 85.f}, .circle = false},
    MaskObject {.point1 = Dir2 {239.f, 85.f}, .point2 = Dir2 {183.f, 135.f}, .circle = false},
    MaskObject {.point1 = Dir2 {183.f, 135.f}, .point2 = Dir2 {280.f, 257.f}, .circle = false},
    MaskObject {.point1 = Dir2 {280.f, 257.f}, .point2 = Dir2 {173.f, 196.f}, .circle = false},
    MaskObject {.point1 = Dir2 {173.f, 196.f}, .point2 = Dir2 {122.f, 84.f}, .circle = false},
    MaskObject {.point1 = Dir2 {364.f, 307.f}, .point2 = Dir2 {531.f, 333.f}, .circle = false},
    MaskObject {.point1 = Dir2 {531.f, 333.f}, .point2 = Dir2 {655.f, 331.f}, .circle = false},
    MaskObject {.point1 = Dir2 {655.f, 331.f}, .point2 = Dir2 {730.f, 379.f}, .circle = false},
    MaskObject {.point1 = Dir2 {730.f, 379.f}, .point2 = Dir2 {718.f, 438.f}, .circle = false},
    MaskObject {.point1 = Dir2 {718.f, 438.f}, .point2 = Dir2 {572.f, 436.f}, .circle = false},
    MaskObject {.point1 = Dir2 {572.f, 436.f}, .point2 = Dir2 {364.f, 307.f}, .circle = false},
    MaskObject {.point1 = Dir2 {554.f, 49.f}, .point2 = Dir2 {646.f, 82.f}, .circle = false},
    MaskObject {.point1 = Dir2 {646.f, 82.f}, .point2 = Dir2 {681.f, 163.f}, .circle = false},
  };
}

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});

  Dir2 origen = {518.f, 334.f};
  const std::vector<MaskObject> segments = get_segments_2();

  bool cont = true;
  SDL_Event event;

  while (cont) {
    glb->begin_render();

    /* The delay must be inside. */
    SDL_Delay(1);

      SDL_SetRenderDrawColor(glb->get_render(), 255, 255, 255, 255);
      for (const auto& segment: segments) {
        SDL_RenderDrawLine(glb->get_render(), segment.point1.x, segment.point1.y, segment.point2.x, segment.point2.y);
      }

      float a = clock();
      std::cout << "position: (" << origen.x << ", " << origen.y << ")";
      std::vector<MaskObject> view = generate_view_covering_by_point(origen, segments);
      std::cout << ", tiempo: " << ((clock() - a) / CLOCKS_PER_SEC);
      std::cout << std::endl;

      SDL_SetRenderDrawColor(glb->get_render(), 255, 0, 0, 255);
      for (const auto& segment: view) {
        SDL_RenderDrawLine(glb->get_render(), segment.point1.x, segment.point1.y, segment.point2.x, segment.point2.y);
      }

      SDL_SetRenderDrawColor(glb->get_render(), 0, 255, 0, 255);
      SDL_RenderDrawPoint(glb->get_render(), origen.x, origen.y);

    glb->end_render();
    
    /* Evaluacion de perifericos. */
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

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    origen = Dir2 {static_cast<float>(mouse_x), static_cast<float>(mouse_y)};
  } 
}
