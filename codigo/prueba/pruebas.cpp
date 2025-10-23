#include "../headers/pr_objects/square.hpp"
#include "../headers/pr_objects/circle.hpp"
#include "../headers/pr_objects/line.hpp"
#include "../headers/concepts/collition.hpp"
#include "../headers/concepts/image_modifier.hpp"
#include "../headers/concepts/visualizer.hpp"
#include "../headers/primitives/global.hpp"

#include <SDL2/SDL.h>
#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>

int main () {
  Global* glb = Global::create("Ventana", 600, 800, SDL_Color {30, 30, 30, 0});
  uint32_t width = glb->get_width();
  uint32_t height = glb->get_height();

  bool cont = true;
  SDL_Event event;

  /* Creacion de objetos movibles. */
  srand(102);
  SDL_Color color = SDL_Color {255,255,255,255};

  std::vector<Circle> circles = std::vector<Circle>();
  Circle cir;
  circles.push_back(Circle(glb, 19, AngDir2 {400, 400, 0}, 2.1, 0, true, true, &color));
  circles.push_back(Circle(glb, 15, AngDir2 {470, 400, 0}, 2.1, 0, true, true, &color));
  circles.push_back(Circle(glb, 25, AngDir2 {400, 470, 0}, 2.1, 0, true, true, &color));
  circles.push_back(Circle(glb, 17, AngDir2 {490, 480, 0}, 2.1, 0, true, true, &color));

  for (auto& cir: circles) {
    cir.set_velocity(AngDir2 {(float)(rand() % 40), (float)(rand() % 40), 0});
  }

  ImageModifier img_mod_2 = ImageModifier::chargePNG("../images/psic1.png");
  ImageModifier img_mod_1 = ImageModifier::circle(15, color);
  Circle c1 = Circle (glb, 15, AngDir2 {120, 60, 0}, 2.1, 0.5, true, true);
  c1.set_texture((img_mod_1 & img_mod_2).cast(glb->get_render()));
  c1.set_velocity(AngDir2 {22, 34, 0});


  /* Creacion de estructura estatica. */

  color = SDL_Color {255,255,255,255};
  img_mod_2 = ImageModifier::chargePNG("../images/psic2.png");
  img_mod_1 = (ImageModifier::square(60, 200, color) & img_mod_2);
  Square c2 = Square(glb, 30, 100, AngDir2 {200, 200, 0}, 4.6, 0.5, false, true, &color);
  c2.set_texture(img_mod_1.resize(200, 60).cast(glb->get_render()));


  /* Creacion de lineas de colision. */

  std::vector<Line> lines = std::vector<Line>();
  lines.push_back(Line (Dir2 {(float)width, 0.f}, Dir2 {0.0f, 0.0f}));
  lines.push_back(Line (Dir2 {(float)width, (float)height}, Dir2 {0.f, (float)height}));

  lines.push_back(Line (Dir2 {0.f, height/2.f}, Dir2 {20.0f, 0.0f}));
  lines.push_back(Line (Dir2 {0.f, height/2.f}, Dir2 {20.0f, (float)height}));

  lines.push_back(Line (Dir2 {(float)width, height/2.f}, Dir2 {(float)width - 20, 0.0f}));
  lines.push_back(Line (Dir2 {(float)width, height/2.f}, Dir2 {(float)width - 20, (float)height}));

  AngDir2 g = AngDir2 {0, 9.8f, 0};

  float b = 15;
  Visualizer<D3FIG> cube = Visualizer<D3FIG>::prism(glb->get_render(), b, b, b);
  cube.set_texture("../images/rubik.png");
  Dir3 cube_pos = Dir3 {500, 200, 100};
  Dir3 cube_rot = Dir3 {0.01, 0.02, 0};

  std::vector<Dir2> polygon_points = std::vector<Dir2>({
    Dir2 {556.f, 464.f},
    Dir2 {664.f, 439.f},
    Dir2 {704.f, 364.f},
    Dir2 {552.f, 300.f},
    Dir2 {578.f, 369.f},
    Dir2 {438.f, 388.f}
  });

  while (cont) {
    glb->begin_render();

    /* The delay must be inside. */
    SDL_Delay(1);

    /* Render of the objects. */
    c1.draw ();
    c2.draw ();
    for (auto& cir: circles)
      cir.draw ();
    cube.draw (glb->get_render(), cube_pos);
    print_polygon_c(glb->get_render(), polygon_points, color);

    /* Calculation of the movement. */
    for (auto& cir: circles)
      cir.calculate_movement(g);
    c1.calculate_movement(g);
    c2.calculate_movement(g);

    /* Testing of the collitions. */
    for (auto& cir: circles) {
      if (test_collition(c2, cir)) 
        resolve_collition(cir, c2);
      if (test_collition(c1, cir)) 
        resolve_collition(cir, c1);
      for (Line& line: lines)
        if (test_collition(cir, line))
          resolve_collition(cir, line);
    }

    for (int i = 0; i < circles.size(); i++)
      for (int j = i + 1; j < circles.size(); j++)
        if (test_collition(circles[i], circles[j]))
          resolve_collition(circles[i], circles[j]);

    if (test_collition(c1, c2)) 
      resolve_collition(c1, c2);
    for (Line& line: lines) {
      if (test_collition(c1, line))
        resolve_collition(c1, line);
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

    cube.rotate(cube_rot);
    glb->end_render();
  } 
}
