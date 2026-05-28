#include "../headers/pr_objects/square.hpp"
#include "../headers/pr_objects/circle.hpp"
#include "../headers/pr_objects/line.hpp"
#include "../headers/concepts/image_modifier.hpp"
#include "../headers/concepts/visualizer.hpp"
#include "../headers/primitives/global.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdlib>
#include <cmath>

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});

  bool cont = true;
  SDL_Event event;

  /* Creacion de objetos movibles. */
  srand(102);
  SDL_Color color = SDL_Color {255,255,255,255};

  ImageModifier img_mod_2 = ImageModifier::chargePNG("../images/psic1.png");
  ImageModifier img_mod_1 = ImageModifier::square(40, 40, color);
  Square c1 = Square(glb, 20, 20, AngDir2 {100, 100, 0}, 2.1, 0.3, true, true);
  c1.set_texture((img_mod_1 & img_mod_2).cast(glb));

  /* Creacion de estructura estatica. */
  color = SDL_Color {255,255,255,255};
  img_mod_2 = ImageModifier::chargePNG("../images/psic2.png");
  img_mod_1 = (ImageModifier::square(60, 200, color) & img_mod_2);
  Square c2 = Square(glb, 30, 60, AngDir2 {100, 400, 0}, 4.6, 0.3, false, true, &color);
  c2.set_texture(img_mod_1.resize(120, 60).rotate180().cast(glb));

  Dir2 point = collition_point(c1, c2);

  while (cont) {
    glb->begin_render();

    c1.draw ();
    c2.draw ();
    SDL_SetRenderDrawColor(glb->get_render(), 255, 0, 0, 255);
    SDL_RenderDrawPoint(glb->get_render(), point.x, point.y);

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
