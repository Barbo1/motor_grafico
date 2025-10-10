#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_stdinc.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>

#include "../headers/pr_objects/square.hpp"
#include "../headers/pr_objects/circle.hpp"
#include "../headers/pr_objects/line.hpp"
#include "../headers/concepts/collition.hpp"
#include "../headers/concepts/image_modifier.hpp"
#include "../headers/concepts/visualizer.hpp"

const char window_name[] = "Ventana";
const uint32_t height = 600;
const uint32_t width = 800;
const SDL_Color bg_color = {30, 30, 30, 0};

int main () {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Could not initialize SDL" << SDL_GetError() << std::endl;
    return -1;
  }
  
  SDL_Window * window = SDL_CreateWindow (
    window_name, 
    SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED, 
    width, 
    height, 
    SDL_WINDOW_SHOWN
  );
  if (window == nullptr) {
    std::cerr << "Could not initialize window" << SDL_GetError() << std::endl;
    SDL_Quit ();
    return -1;
  }

  SDL_Renderer * render = SDL_CreateRenderer (
    window, 
    -1, 
    SDL_RENDERER_ACCELERATED
  );
  if (render == nullptr) {
    std::cerr << "Could not initialize render" << SDL_GetError() << std::endl;
    SDL_DestroyWindow (window);
    SDL_Quit ();
    return -1;
  }

  bool cont = true;
  SDL_Event event;

  /*
  Square c1 = Square(
    render, 15, 15, AngDir2 {120, 120, 0}, 
    SDL_Color {255,255,255,255}, 2.1, 
    0, 0, true, true
  );
  Circle c1 = Circle(
    render, 15, AngDir2 {120, 120, 0}, 
    SDL_Color {255,255,255,255}, 2.1, 
    0, 0, true, true 
  );
  */

  SDL_Color color = SDL_Color {255,255,255,255};
  ImageModifier img_mod_2 = ImageModifier::chargePNG("../images/psic1.png");
  ImageModifier img_mod_1 = ImageModifier::square(30, 30, color);
  Square c1 = Square(
    render, 15, 15, AngDir2 {120, 120, 0}, 
    2.1, 0, 0, true, true
  );
  c1.set_texture((img_mod_1 & img_mod_2).cast(render));
  c1.set_velocity(AngDir2 {34, 34, 0});

  /*
  Square c2 = Square(
    render, 50, 30, AngDir2 {200, 120, 0}, 
    SDL_Color {255,255,255,255}, 4.6, 
    0, 0, true, true
  );

  Circle c2 = Circle(
    render, 30, AngDir2 {200, 120, 0}, 
    SDL_Color {255,255,255,255}, 4.6, 
    0, 0, true, true 
  );
   * */
  color = SDL_Color {255,255,255,255};
  img_mod_2 = ImageModifier::chargePNG("../images/psic2.png");
  img_mod_1 = (ImageModifier::square(60, 200, color) & img_mod_2);
  Square c2 = Square(
    render, 30, 100, AngDir2 {200, 200, 0}, 
    4.6, 0, 0, true, true, &color
  );
  c2.set_texture(img_mod_1.resize(200, 60).cast(render));


  std::vector<Line> lines = std::vector<Line>();
  lines.push_back(Line (Dir2 {width, 0.f}, Dir2 {0.0f, 0.0f}));
  lines.push_back(Line (Dir2 {(float)width, height}, Dir2 {width, 0.f}));
  /*
  lines.push_back(Line (Dir2 {0.f, height}, Dir2 {0.0f, 0.0f}));
  lines.push_back(Line (Dir2 {(float)width, height}, Dir2 {0.f, width}));
  */

  const std::vector<AngDir2 *> external_forces;

  while (cont) {
    SDL_Delay(10);
    SDL_SetRenderDrawColor(render, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    SDL_RenderClear(render);

    c1.calculate_movement(external_forces);

    if (test_collition(c1, c2))
      resolve_collition(c1, c2);
    for (Line& line: lines) {
      if (test_collition(c1, line)) {
        resolve_collition(c1, line);
      }
    }

    c1.draw(render);
    c2.draw(render);
    
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

    SDL_RenderPresent (render);
  } 
  
  SDL_DestroyRenderer (render);
  SDL_DestroyWindow (window);
  SDL_Quit ();
}
