#include "../../headers/primitives/global.hpp"
#include "../../headers/primitives/vectors.hpp"
#include "../../headers/primitives/arena.hpp"
#include "../../headers/concepts/collision.hpp"
#include "../../headers/concepts/image_modifier.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <cstdlib>
#include <array>
#include <iostream>
#include <cmath>

std::array<Dir2, 7> set_points_1 () {
  return std::array<Dir2, 7> {
    Dir2 (219.4f, 404.2f), Dir2 (394.0f, 361.3f), Dir2 (522.9f, 441.6f), 
    Dir2 (520.1f, 244.8f), Dir2 (437.0f, 91.0f), Dir2 (233.2f, 25.9f), 
    Dir2 (291.4f, 170.0f),
  };
}

std::array<Dir2, 11> set_points_2 () {
  return std::array<Dir2, 11> {
    Dir2 (315.6f, 41.4f), Dir2 (219.9f, 200.4f), 
    Dir2 (77.0f, 188.3f), Dir2 (55.5f, 313.6f), Dir2 (246.8f, 359.4f), 
    Dir2 (337.1f, 323.0f), Dir2 (364.1f, 273.2f), Dir2 (424.7f, 209.8f),
    Dir2 (528.5f, 316.3), Dir2 (593.2f, 228.7f), Dir2 (525.8f, 99.3f)
  };
}

std::array<Dir2, 17> set_points_3 () {
  return std::array<Dir2, 17> {
    Dir2 (350.0f, 256.8f), Dir2 (400.f, 200.f), 
    Dir2 (416.8f, 105.4f), Dir2 (485.2f, 147.7f), Dir2 (481.9f, 253.6f), 
    Dir2 (542.2f, 354.5f), Dir2 (625.2f, 256.8f), Dir2 (582.9f, 116.8f), 
    Dir2 (173.1f, 22.3f), Dir2 (34.1f, 159.1f), Dir2 (58.5f, 384.5f), 
    Dir2 (123.6f, 336.6f), Dir2 (113.9f, 279.6f), Dir2 (125.3f, 206.3f), 
    Dir2 (159.5f, 95.6f), Dir2 (226.2f, 103.7f), Dir2 (273.5f, 211.2f), 
  };
}

std::array<Dir2, 10> set_points_4 () {
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

  std::array<Dir2, 10> points = set_points_4();
  NEdgeComp<10> poly(points.data(), points.size(), Dir2 (400.f, 200.f), 2.f, 0.f, true, &error);
  if (error < 0) {
    std::cout << "problema al cargar poligono. (" << error << ")"  << std::endl;
    std::exit(-1);
  }

  struct SquareElement {
    Visualizer<D2FIG> texture;
    Square physical_body;
  };

  SDL_Color color = SDL_Color{.r=0, .g=255, .b=0, .a=255};
  SquareElement sq = SquareElement {
    .texture = ImageModifier::square(40, 100, color).cast(glb),
    .physical_body = Square(AngDir2 (310.f, 450.f, 0.f), 20, 50, 2.f)
  };
  sq.physical_body.velocity.store(AngDir2(0.f, -0.5f, 0.f));
  
  SquareElement sq1 = SquareElement {
    .texture = ImageModifier::square(100, 40, color).cast(glb),
    .physical_body = Square(AngDir2 (460.f, 600.f, 0.f), 50, 30, 2.f)
  };
  sq1.physical_body.velocity.store(AngDir2(0.f, -0.5f, 0.f));

  bool cont = true;
  while (cont) {
    SDL_Delay(16);
    /* The delay must be inside. */

    glb->begin_render();
      poly.physical.print(glb, &gs);
      sq.texture.draw(glb, Dir2(sq.physical_body.position));
      sq1.texture.draw(glb, Dir2(sq1.physical_body.position));

      sq.physical_body.calculate_movement(glb, AngDir2(0.f, 0.f, 0.f));
      sq1.physical_body.calculate_movement(glb, AngDir2(0.f, 0.f, 0.f));
      poly.physical.calculate_movement(glb, AngDir2(0.f, 0.f, 0.f));

      if (test_collision(sq.physical_body, poly.physical)) {
        std::cout << 1 << std::endl;
        Dir2 v = Dir2(sq.physical_body.velocity);
        std::cout << "vel antes = (" << v.x() << ", " << v.y() << ")" << std::endl;
        resolve_collision(sq.physical_body, poly.physical, true);
        v = Dir2(sq.physical_body.velocity);
        std::cout << "vel antes = (" << v.x() << ", " << v.y() << ")" << std::endl;
      }

      if (test_collision(sq1.physical_body, poly.physical)) {
        std::cout << 2 << std::endl;
        resolve_collision(sq1.physical_body, poly.physical, true);
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
