#include "../headers/concepts/image_modifier.hpp"
#include "../headers/concepts/visualizer.hpp"
#include "../headers/primitives/global.hpp"
#include "../headers/sp_objects/particle_source.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath>

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});

  bool cont = true;
  SDL_Event event;

  /* Creacion de objetos movibles. */
  std::vector<Visualizer<D2FIG>> textures;
  textures.push_back (chargePNG (glb, "../images/pngs/basn0g01.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn0g02.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn0g04.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn0g08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn0g16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn2c08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn2c16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn3p01.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn3p02.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn3p04.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn3p08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn4a08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn4a16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn6a08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basn6a16.png"));
  textures.push_back (Visualizer<D2FIG>());

  textures.push_back (chargePNG (glb, "../images/pngs/tbbn0g04.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/tbbn2c16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/tbbn3p08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/tbgn2c16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/tbgn3p08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/tbrn2c08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/tbwn0g16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/tbwn3p08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/tbyn3p08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/tp0n0g08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/tp0n2c08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/tp0n3p08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/tp1n3p08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/tm3n3p02.png"));
  textures.push_back (Visualizer<D2FIG>());

  textures.push_back (chargePNG (glb, "../images/pngs/f00n0g08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/f00n2c08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/f01n0g08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/f01n2c08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/f02n0g08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/f02n2c08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/f03n0g08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/f04n0g08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/f04n2c08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/f99n0g04.png"));
  textures.push_back (Visualizer<D2FIG>());

  textures.push_back (chargePNG (glb, "../images/pngs/pp0n2c16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/pp0n6a08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/ps1n0g08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/ps1n2c16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/ps2n0g08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/ps2n2c16.png"));
  textures.push_back (Visualizer<D2FIG>());

  textures.push_back (chargePNG (glb, "../images/pngs/oi9n2c16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/oi9n0g16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/oi4n2c16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/oi4n0g16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/oi2n2c16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/oi1n2c16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/oi1n0g16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/oi2n0g16.png"));
  textures.push_back (Visualizer<D2FIG>());

  textures.push_back (chargePNG (glb, "../images/pngs/z00n2c08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/z03n2c08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/z06n2c08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/z09n2c08.png"));
  textures.push_back (Visualizer<D2FIG>());

  textures.push_back (chargePNG (glb, "../images/pngs/basi0g01.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi0g02.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi0g04.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi0g08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi0g16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi2c08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi2c16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi3p01.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi3p02.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi3p04.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi3p08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi4a08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi4a16.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi6a08.png"));
  textures.push_back (chargePNG (glb, "../images/pngs/basi6a16.png"));
  textures.push_back (Visualizer<D2FIG>());

/*
  std::cout << "muestra de corrompidos" << std::endl;

  textures.push_back (chargePNG (glb, "../images/pngs/xs1n0g01.png"));
  std::cout << 1 << std::endl;
  textures.push_back (chargePNG (glb, "../images/pngs/xs2n0g01.png"));
  std::cout << 2 << std::endl;
  textures.push_back (chargePNG (glb, "../images/pngs/xs4n0g01.png"));
  std::cout << 3 << std::endl;
  textures.push_back (chargePNG (glb, "../images/pngs/xs7n0g01.png"));
  std::cout << 4 << std::endl;
  textures.push_back (chargePNG (glb, "../images/pngs/xcrn0g04.png"));
  std::cout << 5 << std::endl;
  textures.push_back (chargePNG (glb, "../images/pngs/xlfn1g04.png"));
  std::cout << 6 << std::endl;
  textures.push_back (chargePNG (glb, "../images/pngs/xhdn0g08.png"));
  std::cout << 7 << std::endl;
  textures.push_back (chargePNG (glb, "../images/pngs/xc1n0g08.png"));
  std::cout << 8 << std::endl;
  textures.push_back (chargePNG (glb, "../images/pngs/xc9n2c08.png"));
  std::cout << 9 << std::endl;
  textures.push_back (chargePNG (glb, "../images/pngs/xd0n2c08.png"));
  std::cout << 10 << std::endl;
  textures.push_back (chargePNG (glb, "../images/pngs/xd3n2c08.png"));
  std::cout << 11 << std::endl;
  textures.push_back (chargePNG (glb, "../images/pngs/xd9n2c08.png"));
  std::cout << 12 << std::endl;
  textures.push_back (chargePNG (glb, "../images/pngs/xdtn0g01.png"));
  std::cout << 13 << std::endl;
  textures.push_back (chargePNG (glb, "../images/pngs/xcsn0g01.png"));
  std::cout << 14 << std::endl;
*/

  /* another particle soruce. */
  ParticleSource<PS_EXPLOSION, 10, FT_LINEAR_N, FT_LINEAR_N> parts_1 (
    glb,
    AngDir2 {400.f, 400.f, 0},
    std::pair<float, float>{0.7853, M_PI - 0.7853},
    ImageModifier::square(5, 5, SDL_Color{0, 255, 0, 255}).cast(glb),
    5,
    80.2f,
    1200
  );
  parts_1.burst();

  while (cont) {
    glb->begin_render();
    SDL_Delay(16);

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
 

    float posy = 50;
    float posx = 300;
    for (auto& tex: textures) {
      tex.draw (glb, AngDir2 (posx, posy, 0));
      posy += 50;
      if (posy == 550) {
        posy = 50;
        posx += 50;
      }
    }

    glb->end_render();
  } 
}
