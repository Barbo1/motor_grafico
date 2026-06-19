#include "../headers/primitives/global.hpp"
#include "../headers/primitives/vectors.hpp"
#include "../headers/pr_objects/nedge.hpp"
#include "../headers/pr_objects/circle.hpp"
#include "../headers/concepts/collition.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <cstdlib>
#include <array>
#include <iostream>
#include <cmath>

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});
  SDL_Event event;

  Dir2 A = Dir2(209.32571828122,267.15187548129);
  Dir2 v1 = Dir2(-165.32571828122, -179.15187548129);
  Dir2 v2 = Dir2(-110.0354048893, 060.87064951323);
  Dir2 P;

  bool cont = true;
  while (cont) {
    glb->begin_render();

    std::string dial = "false";
    if (test_collition_triangle_point (A, v1, v2, P)) {
      dial = "true";
    }
    std::cout << dial << std::endl;
    
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    P = Dir2(static_cast<float>(mouse_x), static_cast<float>(mouse_y));
  
    SDL_SetRenderDrawColor(glb->get_render(), 255, 0, 0, 255);
    SDL_RenderDrawPoint(glb->get_render(), P.x, P.y);
    SDL_RenderDrawLine(glb->get_render(), A.x, A.y, (A + v1).x, (A + v1).y);
    SDL_RenderDrawLine(glb->get_render(), A.x, A.y, (A + v2).x, (A + v2).y);
    SDL_RenderDrawLine(glb->get_render(), (A + v2).x, (A + v2).y, (A + v1).x, (A + v1).y);

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

    glb->end_render();
  } 
}
