#include "../../headers/primitives/global.hpp"
#include "../../headers/primitives/glyph_system.hpp"

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
  std::string path = "../fuentes_letras/TrenchThin-aZ1J.ttf";
  GlyphsSystem gs (glb, path, &error);
  if (error != 0) {
    std::exit (-1);
  }

  /*
  char16_t letras[36] = {
    u'a', u'b', u'c', u'd', u'e', u'f', u'g', u'h', u'i',
    u'j', u'k', u'l', u'm', u'n', u'o', u'p', u'q', u'r',
    u's', u't', u'u', u'v', u'w', u'x', u'y', u'z', u'0',
    u'1', u'2', u'3', u'4', u'5', u'6', u'7', u'8', u'9'
  };
  */

  uint32_t pos = 0;
  SDL_Color color_1 = SDL_Color {.r = 255, .g = 0, .b = 255, .a = 255};
  SDL_Color color_2 = SDL_Color {.r = 255, .g = 255, .b = 255, .a = 255};
  gs.cache(u'a', 40, color_1);
  gs.cache(u'b', 40, color_1);
  gs.cache(u'o', 40, color_1);
  std::cout << "uno" << std::endl;
  while (cont) {
    SDL_Delay(1);
    glb->begin_render();
      gs.print (std::u16string(u"ahora se ve mucho mejor."), 40, color_1, Dir2 {0.f, 40.f});
      gs.print (std::u16string(u"incluso con distintos tamaños."), 30, color_2, Dir2 {0.f, 80.f});
      gs.print (std::u16string(u"Jovencillo emponzoñado de whisky: ¡que figurota exhibe!"), 20, color_1, Dir2 {0.f, 120.f});
      //gs.print(letras[pos], 320, Dir2 {0.f, 0.f});
      //gs.print(u'ä', 320, Dir2 {0.f, 0.f});
      //Dir2 res;
      //res = Dir2(138.38, 76.5).round();
      //SDL_SetRenderDrawColor(glb->get_render(), 255, 0, 0, 255);
      //SDL_RenderDrawPoint (glb->get_render(), static_cast<uint64_t>(res.x), static_cast<uint64_t>(res.y));
      //res = Dir2(134.64, 75.14).round();
      //SDL_SetRenderDrawColor(glb->get_render(), 0, 255, 0, 255);
      //SDL_RenderDrawPoint (glb->get_render(), static_cast<uint64_t>(res.x), static_cast<uint64_t>(res.y));
      //res = Dir2(130.56, 75.14).round();
      //SDL_SetRenderDrawColor(glb->get_render(), 0, 255, 255, 255);
      //SDL_RenderDrawPoint (glb->get_render(), static_cast<uint64_t>(res.x), static_cast<uint64_t>(res.y));

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
