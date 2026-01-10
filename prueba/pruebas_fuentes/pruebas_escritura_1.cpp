#include "../../headers/primitives/global.hpp"
#include "../../headers/primitives/glyph_system.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <cmath>
#include <string>
#include <cstring>
#include <iostream>

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});

  bool cont = true;
  SDL_Event event;

  int error;
  std::string path = "../fuentes_letras/Nostard-Medium.ttf";
  GlyphsSystem gs (glb, path, &error);
  if (error != 0) {
    std::exit (-1);
  }

  while (cont) {
    SDL_Delay(1);
    glb->begin_render();
      gs.print (std::u16string(u"ahora se ve mucho mejor."), 40, Dir2 {0.f, 40.f});
      gs.print (std::u16string(u"incluso con distintos tamanios."), 30, Dir2 {0.f, 80.f});
      gs.print (std::u16string(u"Jovencillo emponzoñado de whisky: ¡que figurota exhibe!"), 20, Dir2 {0.f, 120.f});
      //gs.print(u'd', 20, Dir2 {0.f, 0.f});
      //gs.print(u'd', 10, Dir2 {0.f, 250.f});
      //gs.print(u'd', 15, Dir2 {250.f, 0.f});
      //gs.print(u'd', 30, Dir2 {220.f, 200.f});
      //gs.print(u'd', 18, Dir2 {560.f, 200.f});
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
