#include "../../headers/primitives/global.hpp"
#include "../../headers/primitives/vectors.hpp"
#include "../../headers/pr_objects/nedge.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <cstdlib>
#include <array>
#include <iostream>
#include <cmath>

bool test_collition_triangle_line (Dir2 A, Dir2 v_b, Dir2 v_c, Dir2 D, Dir2 v_e) {
  bool c1 = v_b.pLd (v_e, v_c) > 0.f;
  bool c2 = v_c.pLd (v_e, v_b) > 0.f;
  bool c3 = c1 + c2 < 0.f;
  float v1 = v_b.pLd(A - D, v_e);
  float v2 = v_c.pLd(A - D, v_e);
  float v3 = (v_c - v_b).pLd(v_b - (A - D), v_e);

  float c_I = std::max (c1 ? v1 : 0.f, std::max (c2 ? v2 : 0.f, c3 ? v3 : 0.f));
  float c_S = std::min (!c1 ? v1 : 1.f, std::min (!c2 ? v2 : 1.f, !c3 ? v3 : 1.f));

  return c_S > c_I;
}

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

  std::array<Dir2, 7> points = set_points_1();
  NEdge<7> poly(glb, points.data(), points.size(), nullptr, &error);
  if (error < 0) {
    std::cout << "problema al cargar poligono." << std::endl;
    std::exit(-1);
  }

  bool cont = true;
  while (cont) {
    glb->begin_render();

    poly.print(glb, &gs);

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
