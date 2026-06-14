#include "../../headers/primitives/global.hpp"
#include "../../headers/primitives/vectors.hpp"
#include "../../headers/pr_objects/nedge.hpp"
#include "../../headers/pr_objects/circle.hpp"
#include "../../headers/concepts/collition.hpp"

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
  SDL_Event event;
  
  int32_t error;
  GlyphsSystem gs (glb, "../fuentes_letras/Nostard-Medium.ttf", &error);
  if (error < 0) {
    std::cout << "problema al cargar fuentes de letra." << std::endl;
    std::exit(-1);
  }

  std::array<Dir2, 4> points = set_points_1();
  NEdge<4> poly(
    glb, points.data(), points.size(), Dir2 (400.f, 200.f), 2.f, 0.f, true, true,
    nullptr, &error
  );
  if (error < 0) {
    std::cout << "problema al cargar poligono. (" << error << ")"  << std::endl;
    std::exit(-1);
  }

  SDL_Color color = SDL_Color{.r=0, .g=255, .b=0, .a=255};
  Circle cir = Circle (glb, 30, Dir2 {360.f, 450.f}, 2.f, 0.f, true, true, &color);
  cir.set_velocity(AngDir2(0.f, -60.f, 0.f));
  
  Circle cir1 = Circle (glb, 20, Dir2 {460.f, 600.f}, 2.f, 0.f, true, true, &color);
  cir1.set_velocity(AngDir2(0.f, -100.f, 0.f));

  bool cont = true;
  while (cont) {
    /* The delay must be inside. */
    SDL_Delay(1);

    glb->begin_render();
      poly.print(glb, &gs);
      cir.draw();
      cir1.draw();
    glb->end_render();

    cir.calculate_movement(AngDir2(0.f, 0.f, 0.f));
    cir1.calculate_movement(AngDir2(0.f, 0.f, 0.f));
    poly.calculate_movement(AngDir2(0.f, 0.f, 0.f));

    if (test_collition(cir, poly)) {
      resolve_collition(cir, poly);
    }

    if (test_collition(cir1, poly)) {
      resolve_collition(cir1, poly);
    }

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
