#include "../../headers/concepts/image_modifier.hpp"
#include "../../headers/concepts/visualizer.hpp"
#include "../../headers/primitives/global.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <cstdlib>
#include <cmath>

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});

  bool cont = true;
  SDL_Event event;

  SDL_Color color1 = SDL_Color {255, 255, 255, 255};
  SDL_Color color2 = SDL_Color {255, 0, 0, 255};
  Visualizer<D2FIG> tex_1 = ImageModifier::square(10, 70, color1).cast(glb);
  Visualizer<D2FIG> tex_2 = ImageModifier::square(40, 40, color2).cast(glb);
  Visualizer<D2FIG> tex_3 = ImageModifier::circle(10, color2).cast(glb);
  Visualizer<D2FIG> tex_4 = ImageModifier::circle(7, color2).cast(glb);
  Visualizer<D2FIG> tex_5 = ImageModifier::rounded_square(100, 150, 13, color1).cast(glb);

  ImageModifier image = ImageModifier::chargePNG("../images/minecraft.png");
  Visualizer<D2FIG> tex_6 = image.subimage(Dir2(20.f, 20.f), Dir2(20.f, 20.f)).cast(glb);
  Visualizer<D2FIG> tex_7 = image.cast(glb);

  /* Creacion de lineas de colision. */
  while (cont) {
    glb->begin_render();
    tex_1.draw(glb, Dir2 (200.f, 100.f));
    tex_2.draw(glb, Dir2 (200.f, 200.f));
    tex_3.draw(glb, Dir2 (200.f, 400.f));
    tex_4.draw(glb, Dir2 (200.f, 500.f));
    tex_5.draw(glb, Dir2 (400.f, 100.f));
    tex_6.draw(glb, Dir2 (400.f, 250.f));
    tex_7.draw(glb, Dir2 (400.f, 300.f));
    
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
