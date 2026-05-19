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

const Visualizer<D2FIG> slider_sign = 
  ImageModifier::square(20, 6, SDL_Color {.r=200, .g=200, .b=200, .a=255})
  .cast(glb);
const Visualizer<D2FIG> slider_base = 
  ImageModifier::square(4, 100, SDL_Color {.r=255, .g=0, .b=0, .a=255})
  .cast(glb);

const Visualizer<D2FIG> slider_sign_2 = 
  ImageModifier::square(6, 20, SDL_Color {.r=200, .g=200, .b=200, .a=255})
  .cast(glb);
const Visualizer<D2FIG> slider_base_2 = 
  ImageModifier::square(100, 4, SDL_Color {.r=255, .g=0, .b=0, .a=255})
  .cast(glb);

const Visualizer<D2FIG> button_quiet = 
  ImageModifier::square(20, 40, SDL_Color {.r=0, .g=0, .b=255, .a=255})
  .cast(glb);
const Visualizer<D2FIG> button_observed = 
  ImageModifier::square(20, 40, SDL_Color {.r=255, .g=0, .b=0, .a=255})
  .cast(glb);
const Visualizer<D2FIG> button_selected = 
  ImageModifier::square(20, 40, SDL_Color {.r=0, .g=255, .b=0, .a=255})
  .cast(glb);

const Visualizer<D2FIG> textbox_background =
  ImageModifier::square(20, 90, SDL_Color {.r=0, .g=0, .b=0, .a=255}).
  cast(glb);

void print_checkbox_deactivate(Dir2 pos) {
  checkbox_deactivate.draw(glb, pos);
}

void print_checkbox_activate(Dir2 pos) {
  checkbox_activate.draw(glb, pos);
}

void print_slider_base(Dir2 pos) {
  slider_base.draw(glb, pos);
}

void print_slider_sign(Dir2 pos) {
  slider_sign.draw(glb, pos);
}

void print_slider_base_2(Dir2 pos) {
  slider_base_2.draw(glb, pos);
}

void print_slider_sign_2(Dir2 pos) {
  slider_sign_2.draw(glb, pos);
}

void print_button_quiet(Dir2 pos) {
  button_quiet.draw(glb, pos);
}

void print_button_observed(Dir2 pos) {
  button_observed.draw(glb, pos);
}

void print_button_selected(Dir2 pos) {
  button_selected.draw(glb, pos);
}

void print_texbox (Dir2 pos) {
  textbox_background.draw(glb, pos);
}

int main () {
  bool cont = true;
  SDL_Event event;

  CheckBox check = CheckBox (
    print_checkbox_activate, 
    print_checkbox_deactivate,
    Dir2(200.f, 200.f),
    Dir2(40.f, 40.f),
    false
  );

  Slider slider_1 = Slider (
    print_slider_base,
    print_slider_sign,
    Dir2(200.f, 300.f),
    Dir2 (100.f, 16.f),
    Dir2(6.f, 20.f),
    1,
    SliderDirectionX
  );
  
  Slider slider_2 = Slider (
    print_slider_base_2,
    print_slider_sign_2,
    Dir2(200.f, 400.f),
    Dir2 (16.f, 100.f),
    Dir2(20.f, 6.f),
    6,
    SliderDirectionY
  );

  Button button = Button (
    print_button_selected,
    print_button_observed,
    print_button_quiet,
    Dir2(300.f, 200.f),
    Dir2(40.f, 20.f)
  );

  TextBox textbox = TextBox (
    print_texbox,
    Dir2(400.f, 300.f),
    Dir2(90.f, 20.f),
    16,
    SDL_Color {.r=255, .g=255, .b=255, .a=255},
    15,
    false
  );

  std::vector<SDL_KeyCode> keys = std::vector<SDL_KeyCode> {
    SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f, SDLK_g, SDLK_h, 
    SDLK_i, SDLK_j, SDLK_k, SDLK_l, SDLK_m, SDLK_n, SDLK_o, SDLK_p, 
    SDLK_q, SDLK_r, SDLK_s, SDLK_t, SDLK_u, SDLK_v, SDLK_w, SDLK_x, 
    SDLK_y, SDLK_z
  };

  GuiComponent<5> inicio = GuiComponent<5>(glb, &gs, keys, &error);
  if (error != 0) {
    std::cout << "there is a problem with the gui" << std::endl;
    std::exit(-1);
  }

  inicio.add(&check);
  inicio.add(&slider_1);
  inicio.add(&slider_2);
  inicio.add(&button);
  inicio.add(&textbox);

  /* Creacion de lineas de colision. */
  while (cont) {

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
      inicio.test();
      inicio.print();
    glb->end_render();
  } 
}
