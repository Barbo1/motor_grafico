#include "../../headers/concepts/image_modifier.hpp"
#include "../../headers/concepts/visualizer.hpp"
#include "../../headers/concepts/gui.hpp"
#include "../../headers/primitives/global.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <cmath>
  
Global* glb = Global::create("Ventana", 600, 800, SDL_Color {30, 30, 30, 0});

int error;
GlyphsSystem gs (glb, "../fuentes_letras/Nostard-Medium.ttf", &error);

const Visualizer<D2FIG> checkbox_deactivate = 
  ImageModifier::chargePNG("../images/checkbox_deactivate.png")
  .resize(40, 40)
  .cast(glb);
const Visualizer<D2FIG> checkbox_activate = 
  ImageModifier::chargePNG("../images/checkbox_activate.png")
  .resize(40, 40)
  .cast(glb);

void print_checkbox_deactivate(Dir2 pos) {
  checkbox_deactivate.draw(glb, pos);
}

void print_checkbox_activate(Dir2 pos) {
  checkbox_activate.draw(glb, pos);
}

int main () {
  bool cont = true;
  SDL_Event event;

  CheckBox check = CheckBox {
    .active_fn = print_checkbox_activate, 
    .deactive_fn = print_checkbox_deactivate,
    .dims = Dir2(40.f, 40.f),
    .position = Dir2(200.f, 200.f),
    .active = false,
    .last_state = GUIStateQuiet 
  };

  GuiComponent<1> inicio = GuiComponent<1>(glb, &gs);
  inicio.add(&check);

  /* Creacion de lineas de colision. */
  while (cont) {
    Dir2 click_position;
    int x, y;
    bool clicking;

    uint32_t mask = SDL_GetMouseState(&x, &y);
    clicking = mask & SDL_BUTTON(SDL_BUTTON_LEFT);
    click_position = Dir2(x, y);

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
    
    glb->begin_render();
      inicio.test_selected(click_position, clicking);
      inicio.print();
    glb->end_render();
  } 
}
