#include "../../headers/concepts/image_modifier.hpp"
#include "../../headers/concepts/visualizer.hpp"
#include "../../headers/primitives/global.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <cmath>

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});

  bool cont = true;
  SDL_Event event;

  /* Creacion de objetos movibles. */
  Visualizer<D2FIG> tex_1, tex_2, tex_3;

  {
    ImageModifier img_mod_0 = ImageModifier::chargePNG("../images/rubik.png");

    float time_get = (float)SDL_GetTicks();
    tex_1 = img_mod_0.cast(glb);
    tex_2 = img_mod_0.rbpp(6).cast(glb);
    tex_3 = img_mod_0.rbpp(7).cast(glb);
    std::cout << "tardo: " << ((SDL_GetTicks() - time_get) / 1000.f) << std::endl;
  }

  /* Creacion de lineas de colision. */
  while (cont) {
    glb->begin_render();
    tex_1.draw(glb, Dir2 {200.f, 100.f});
    tex_2.draw(glb, Dir2 {200.f, 400.f});
    tex_3.draw(glb, Dir2 {500.f, 100.f});
    SDL_SetRenderDrawColor(glb->get_render(), 255, 0, 0, 255);
    SDL_RenderDrawPoint(glb->get_render(), 200, 200);
    
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
