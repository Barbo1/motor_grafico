#include "../../headers/primitives/global.hpp"
#include "../../headers/primitives/vectors.hpp"
#include "../../headers/pr_objects/nedge.hpp"
#include "../../headers/pr_objects/circle.hpp"
#include "../../headers/concepts/collision.hpp"
#include "../../headers/concepts/image_modifier.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <cstdlib>
#include <array>
#include <iostream>
#include <cmath>

std::array<Dir2, 4> set_points_1 () {
  return std::array<Dir2, 4> {
    Dir2 (100.f, 20.f), Dir2 (-100.f, 20.f), Dir2 (-100.f, -20.f), Dir2 (100.f, -20.f)
  };
}

std::array<Dir2, 10> set_points_2 () {
  return std::array<Dir2, 10> {
    Dir2 (-26.1f, 39.0f), Dir2 (22.3f, 36.5f), Dir2 (50.8f, -6.4f),
    Dir2 (22.7f, -9.f), Dir2 (27.8f, -26.8), Dir2 (-23.5f, -37.0f),
    Dir2 (-11.6f, -23.0f), Dir2 (-14.6f, 3.8f), Dir2 (-6.0f, 6.3f),
    Dir2 (-57.1f, 18.6f)
  };
}


int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});
  Arena arena = Arena(1000*1000*4);
  SDL_Event event;
  
  int32_t error;
  GlyphsSystem gs (glb, &arena, "../fuentes_letras/Nostard-Medium.ttf", &error);
  if (error < 0) {
    std::cout << "problema al cargar fuentes de letra." << std::endl;
    std::exit(-1);
  }

  std::array<Dir2, 10> points = set_points_2();
  NEdge<10> poly(points.data(), points.size(), AngDir2 (400.f, 200.f, 0.f), 2.f, 0.f, true, &error);
  if (error < 0) {
    std::cout << "problema al cargar poligono. (" << error << ")"  << std::endl;
    std::exit(-1);
  }

  struct CircleElement {
    Visualizer<D2FIG> texture;
    Circle physical_body;
  };

  SDL_Color color = SDL_Color{.r=0, .g=255, .b=0, .a=255};
  CircleElement cir = CircleElement {
    .texture = ImageModifier::circle(arena, 30, color).cast(glb),
    .physical_body = Circle (Dir2(360.f, 450.f), 30.f, 2.f, 0.f, true)
  };
  cir.physical_body.velocity.store(AngDir2(0.f, -0.3f, 0.f));

  CircleElement cir1 = CircleElement {
    .texture = ImageModifier::circle(arena, 20, color).cast(glb),
    .physical_body = Circle (AngDir2(480.f, 600.f, 0.f), 20.f, 2.f, 0.f, true)
  };
  cir1.physical_body.velocity.store(AngDir2(0.f, -0.5f, 0.f));

  bool cont = true;
  while (cont) {
    SDL_Delay(16);
    /* The delay must be inside. */

    glb->begin_render();
      poly.print(glb, &gs);
      cir.texture.draw(glb, Dir2(cir.physical_body.position));
      cir1.texture.draw(glb, Dir2(cir1.physical_body.position));

      cir.physical_body.calculate_movement(glb, AngDir2(0.f, 0.f, 0.f));
      cir1.physical_body.calculate_movement(glb, AngDir2(0.f, 0.f, 0.f));
      poly.calculate_movement(glb, AngDir2(0.f, 0.f, 0.f));

      if (test_collision(cir.physical_body, poly)) {
        resolve_collision(cir.physical_body, poly, true);
      }

      if (test_collision(cir1.physical_body, poly)) {
        resolve_collision(cir1.physical_body, poly, true);
      }

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
  } 
}
