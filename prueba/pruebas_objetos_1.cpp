#include "../headers/pr_objects/square.hpp"
#include "../headers/pr_objects/circle.hpp"
#include "../headers/pr_objects/line.hpp"
#include "../headers/concepts/collition.hpp"
#include "../headers/concepts/image_modifier.hpp"
#include "../headers/concepts/visualizer.hpp"
#include "../headers/primitives/global.hpp"
#include "../headers/sp_objects/impulse.hpp"
#include "../headers/sp_objects/particle_source.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath>

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});
  uint32_t width = glb->get_width();
  uint32_t height = glb->get_height();

  bool cont = true;
  SDL_Event event;

  /* Creacion de objetos movibles. */
  SDL_Color color = SDL_Color {255,255,255,255};

  ImageModifier img_mod_2 = ImageModifier::chargePNG("../images/psic1.png");
  ImageModifier img_mod_1 = ImageModifier::square(40, 40, color);
  Square c1 = Square(glb, 20, 20, AngDir2 {50, 60, 0}, 2.1, 0.3, true, true);
  c1.set_texture((img_mod_1 & img_mod_2).cast(glb));
  c1.set_velocity(AngDir2 {3, 34, 0});


  /* Creacion de estructura estatica. */

  color = SDL_Color {255,255,255,255};
  img_mod_2 = ImageModifier::chargePNG("../images/psic2.png");
  img_mod_1 = (ImageModifier::square(60, 200, color) & img_mod_2);
  Square c2 = Square(glb, 30, 200, AngDir2 {200, 200, 0}, 4.6, 0.3, false, true, &color);
  c2.set_texture(img_mod_1.resize(400, 60).rotate180().cast(glb));


  /* Creacion de lineas de colision. */

  std::vector<Line> lines = std::vector<Line>();
  lines.push_back(Line (Dir2 {(float)width, 0.f}, Dir2 {0.0f, 0.0f}));
  lines.push_back(Line (Dir2 {(float)width, (float)height}, Dir2 {0.f, (float)height}));

  lines.push_back(Line (Dir2 {0.f, height/2.f}, Dir2 {20.0f, 0.0f}));
  lines.push_back(Line (Dir2 {0.f, height/2.f}, Dir2 {20.0f, (float)height}));

  lines.push_back(Line (Dir2 {(float)width, height/2.f}, Dir2 {(float)width - 20, 0.0f}));
  lines.push_back(Line (Dir2 {(float)width, height/2.f}, Dir2 {(float)width - 20, (float)height}));

  AngDir2 g = AngDir2 {0, 9.8f, 0};

  float b = 40;
  Visualizer<D3FIG> cube = Visualizer<D3FIG>::prism(glb, b, b, b);
  cube.set_texture(ImageModifier::chargePNG("../images/rubik.png"));
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
    /* The delay must be inside. */
    SDL_Delay(1);

    /* Render of the objects. */
    glb->begin_render();
      print_polygon_c(glb, polygon_points, SDL_Color {255, 255, 0, 255});
      c1.draw ();
      c2.draw ();
      cube.draw (glb, cube_pos);
    glb->end_render();

    /* Calculation of the movement. */
    c1.calculate_movement(g);

    /* Testing of the collitions. */
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
  } 
}
