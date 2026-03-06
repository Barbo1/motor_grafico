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
  Visualizer<D2FIG> tex_1, tex_2, tex_3, tex_4;

  {
    ImageModifier img_mod_0 = ImageModifier::chargePNG("../images/sky.png");
    tex_1 = img_mod_0.cast(glb);
    float time_get = clock();
    img_mod_0.ymirror();
    std::cout << "tardo: " << ((clock() - time_get) / CLOCKS_PER_SEC) << std::endl;
    tex_2 = img_mod_0.cast(glb);
  }

  /* Creacion de lineas de colision. */
  while (cont) {
    glb->begin_render();
    tex_1.draw(glb, Dir2 {200.f, 100.f});
    tex_2.draw(glb, Dir2 {200.f, 300.f});
    SDL_SetRenderDrawColor(glb->get_render(), 255, 255, 255, 255);
    SDL_RenderDrawPoint(glb->get_render(), 200, 300);
    
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
