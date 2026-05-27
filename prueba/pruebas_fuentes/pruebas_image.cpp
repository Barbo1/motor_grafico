#include "../../headers/primitives/global.hpp"
#include "../../headers/concepts/glyph_system.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <cmath>
#include <cstdint>
#include <string>
#include <cstring>
#include <iostream>

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});

  bool cont = true;
  SDL_Event event;

  int error;
  /*
    ArianaVioleta-dz2K
    Nostard-Medium
    Aloevera-OVoWO
    TrenchThin-aZ1J
  */
  std::string path = "../fuentes_letras/Nostard-Medium.ttf";
  GlyphsSystem gs (glb, path, &error);
  if (error != 0) {
    std::exit (-1);
  }

  uint32_t pos = 0;
  SDL_Color color_1 = SDL_Color {.r = 255, .g = 0, .b = 255, .a = 255};
  gs.cache(u'a', 40, color_1);
  gs.cache(u'b', 40, color_1);
  gs.cache(u'o', 40, color_1);
  Dir2 dims = Dir2(100, 30);
  Dir2 posv = Dir2(100.f, 100.f);
  Visualizer<D2FIG> phrase = gs.image("hola nose", 25, SDL_Color{.r=255, .g=255, .b=0, .a=255}, dims);
  while (cont) {
    SDL_Delay(1);
    glb->begin_render();
      phrase.draw(glb, posv);
      SDL_SetRenderDrawColor(glb->get_render(), 255, 0, 0, 255);
      SDL_RenderDrawPoint(glb->get_render(), posv.x - dims.x*0.5f, posv.y - dims.y*0.5f);
      SDL_RenderDrawPoint(glb->get_render(), posv.x - dims.x*0.5f, posv.y + dims.y*0.5f);
      SDL_RenderDrawPoint(glb->get_render(), posv.x + dims.x*0.5f, posv.y - dims.y*0.5f);
      SDL_RenderDrawPoint(glb->get_render(), posv.x + dims.x*0.5f, posv.y + dims.y*0.5f);
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
            case SDLK_SPACE:
              pos = (pos + 1) % 36;
              break;
          }
          break;
      }
    }
  } 
}
