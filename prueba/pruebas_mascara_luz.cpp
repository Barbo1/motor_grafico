#include "../headers/concepts/lights.hpp"
#include "../headers/primitives/global.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <iostream>

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});

  Dir2 origen = {665.f, 376.f};
  const std::vector<MaskObject> segments = {
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
    /*
    MaskObject {.point1 = Dir2 {}, .point2 = Dir2 {}, .circle = false},
    MaskObject {.point1 = Dir2 {}, .point2 = Dir2 {}, .circle = false},
    MaskObject {.point1 = Dir2 {}, .point2 = Dir2 {}, .circle = false},
    */
  };

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
      std::vector<MaskObject> view = generate_view_covering(origen, segments);
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
