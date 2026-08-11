#include "../../headers/concepts/lights.hpp"
#include "../../headers/concepts/glyph_system.hpp"
#include "../../headers/concepts/image_modifier.hpp"
#include "../../headers/primitives/global.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <vector>
#include <cmath>
#include <iostream>

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

std::vector<MaskObject> get_segments_1 () {
  return {
    MaskObject {.point1 = Dir2 {134.f, 114.f}, .point2 = Dir2 {52.f, 274.f}, .circle = false},
    MaskObject {.point1 = Dir2 {245.f, 103.f}, .point2 = Dir2 {604.f, 76.f}, .circle = false},
    MaskObject {.point1 = Dir2 {678.f, 149.f}, .point2 = Dir2 {411.f, 178.f}, .circle = false},
    MaskObject {.point1 = Dir2 {117.f, 355.f}, .point2 = Dir2 {278.f, 195.f}, .circle = false},
    MaskObject {.point1 = Dir2 {531.f, 218.f}, .point2 = Dir2 {634.f, 210.f}, .circle = false},
    MaskObject {.point1 = Dir2 {566.f, 441.f}, .point2 = Dir2 {382.f, 313.f}, .circle = false},
    MaskObject {.point1 = Dir2 {617.f, 466.f}, .point2 = Dir2 {388.f, 437.f}, .circle = false},
    MaskObject {.point1 = Dir2 {121.f, 265.f}, .point2 = Dir2 {172.f, 199.f}, .circle = false},
    MaskObject {.point1 = Dir2 {464.f, 348.f}, .point2 = Dir2 {447.f, 337.f}, .circle = false},
    MaskObject {.point1 = Dir2 {498.f, 370.f}, .point2 = Dir2 {506.f, 377.f}, .circle = false},
    MaskObject {.point1 = Dir2 {529.f, 390.f}, .point2 = Dir2 {546.f, 407.f}, .circle = false},
    MaskObject {.point1 = Dir2 {540.f, 320.f}, .point2 = Dir2 {595.f, 355.f}, .circle = false},
  };
}

std::vector<MaskObject> get_segments_2 () {
  return {
    MaskObject {.point1 = Dir2 {122.f, 84.f}, .point2 = Dir2 {86.f, 125.f}, .circle = false},
    MaskObject {.point1 = Dir2 {86.f, 125.f}, .point2 = Dir2 {53.f, 1.78f}, .circle = false},
    MaskObject {.point1 = Dir2 {53.f, 1.78f}, .point2 = Dir2 {62.f, 221.f}, .circle = false},
    MaskObject {.point1 = Dir2 {62.f, 221.f}, .point2 = Dir2 {121.f, 196.f}, .circle = false},
    MaskObject {.point1 = Dir2 {121.f, 196.f}, .point2 = Dir2 {112.f, 243.f}, .circle = false},
    MaskObject {.point1 = Dir2 {112.f, 243.f}, .point2 = Dir2 {20.f, 302.f}, .circle = false},
    MaskObject {.point1 = Dir2 {20.f, 302.f}, .point2 = Dir2 {50.f, 360.f}, .circle = false},
    MaskObject {.point1 = Dir2 {50.f, 360.f}, .point2 = Dir2 {106.f, 330.f}, .circle = false},
    MaskObject {.point1 = Dir2 {106.f, 330.f}, .point2 = Dir2 {60.f, 315.f}, .circle = false},
    MaskObject {.point1 = Dir2 {60.f, 315.f}, .point2 = Dir2 {108.f, 293.f}, .circle = false},
    MaskObject {.point1 = Dir2 {108.f, 293.f}, .point2 = Dir2 {164.f, 311.f}, .circle = false},
    MaskObject {.point1 = Dir2 {164.f, 311.f}, .point2 = Dir2 {110.f, 378.f}, .circle = false},
    MaskObject {.point1 = Dir2 {110.f, 378.f}, .point2 = Dir2 {40.f, 429.f}, .circle = false},
    MaskObject {.point1 = Dir2 {40.f, 429.f}, .point2 = Dir2 {37.f, 470.f}, .circle = false},
    MaskObject {.point1 = Dir2 {37.f, 470.f}, .point2 = Dir2 {127.f, 502.f}, .circle = false},
    MaskObject {.point1 = Dir2 {127.f, 502.f}, .point2 = Dir2 {330.f, 468.f}, .circle = false},
    MaskObject {.point1 = Dir2 {330.f, 468.f}, .point2 = Dir2 {157.f, 438.f}, .circle = false},
    MaskObject {.point1 = Dir2 {157.f, 438.f}, .point2 = Dir2 {282.f, 357.f}, .circle = false},
    MaskObject {.point1 = Dir2 {282.f, 357.f}, .point2 = Dir2 {546.f, 482.f}, .circle = false},
    MaskObject {.point1 = Dir2 {546.f, 482.f}, .point2 = Dir2 {765.f, 473.f}, .circle = false},
    MaskObject {.point1 = Dir2 {765.f, 473.f}, .point2 = Dir2 {750.f, 245.f}, .circle = false},
    MaskObject {.point1 = Dir2 {750.f, 245.f}, .point2 = Dir2 {703.f, 284.f}, .circle = false},
    MaskObject {.point1 = Dir2 {703.f, 284.f}, .point2 = Dir2 {500.f, 200.f}, .circle = false},
    MaskObject {.point1 = Dir2 {500.f, 200.f}, .point2 = Dir2 {532.f, 287.f}, .circle = false},
    MaskObject {.point1 = Dir2 {532.f, 287.f}, .point2 = Dir2 {409.f, 230.f}, .circle = false},
    MaskObject {.point1 = Dir2 {409.f, 230.f}, .point2 = Dir2 {446.f, 104.f}, .circle = false},
    MaskObject {.point1 = Dir2 {446.f, 104.f}, .point2 = Dir2 {269.f, 139.f}, .circle = false},
    MaskObject {.point1 = Dir2 {269.f, 139.f}, .point2 = Dir2 {250.f, 117.f}, .circle = false},
    MaskObject {.point1 = Dir2 {250.f, 117.f}, .point2 = Dir2 {239.f, 85.f}, .circle = false},
    MaskObject {.point1 = Dir2 {239.f, 85.f}, .point2 = Dir2 {183.f, 135.f}, .circle = false},
    MaskObject {.point1 = Dir2 {183.f, 135.f}, .point2 = Dir2 {280.f, 257.f}, .circle = false},
    MaskObject {.point1 = Dir2 {280.f, 257.f}, .point2 = Dir2 {173.f, 196.f}, .circle = false},
    MaskObject {.point1 = Dir2 {173.f, 196.f}, .point2 = Dir2 {122.f, 84.f}, .circle = false},
    MaskObject {.point1 = Dir2 {364.f, 307.f}, .point2 = Dir2 {531.f, 333.f}, .circle = false},
    MaskObject {.point1 = Dir2 {531.f, 333.f}, .point2 = Dir2 {655.f, 331.f}, .circle = false},
    MaskObject {.point1 = Dir2 {655.f, 331.f}, .point2 = Dir2 {730.f, 379.f}, .circle = false},
    MaskObject {.point1 = Dir2 {730.f, 379.f}, .point2 = Dir2 {718.f, 438.f}, .circle = false},
    MaskObject {.point1 = Dir2 {718.f, 438.f}, .point2 = Dir2 {572.f, 436.f}, .circle = false},
    MaskObject {.point1 = Dir2 {572.f, 436.f}, .point2 = Dir2 {364.f, 307.f}, .circle = false},
    MaskObject {.point1 = Dir2 {554.f, 49.f}, .point2 = Dir2 {646.f, 82.f}, .circle = false},
    MaskObject {.point1 = Dir2 {646.f, 82.f}, .point2 = Dir2 {681.f, 163.f}, .circle = false},
  };
}

std::vector<MaskObject> get_segments_3 () {
  return {
    MaskObject {.point1 = Dir2 {134.f, 114.f}, .point2 = Dir2 {52.f, 274.f}, .circle = false},
  };
}

void configure_glyph_system (GlyphsSystem* gs, uint32_t size, SDL_Color color) {
  constexpr std::array<char16_t, 26 + 26 + 10 + 8 + 2> characters = {
    u'A', u'B', u'C', u'D', u'E', u'F', u'G', u'H', u'I', u'J',
    u'K', u'L', u'M', u'N', u'O', u'P', u'Q', u'R', u'S', u'T',
    u'U', u'V', u'W', u'X', u'Y', u'Z',

    u'a', u'b', u'c', u'd', u'e', u'f', u'g', u'h', u'i', u'j',
    u'k', u'l', u'm', u'n', u'o', u'p', u'q', u'r', u's', u't',
    u'u', u'v', u'w', u'x', u'y', u'z',

    u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7', u'8', u'9',

    u'_', u'?', u'!', u':', u',', u'.', u'/', u'(', u')', u' '
  };
  for (const auto& character: characters)
    gs->cache(character, size, color);
  gs->clear_meta();
}

int main () {
  Global* glb = Global::create("hola", SDL_Color {30, 30, 30, 0});
  uint32_t width = glb->get_width();
  Arena arena = Arena(1000*1000*8);
  
  int32_t error;
  SDL_Color color = SDL_Color {255,255,255,255};
  GlyphsSystem gs (glb, &arena, "../fuentes_letras/Nostard-Medium.ttf", &error);
  if (error < 0) {
    std::cout << "problema al cargar fuentes de letra." << std::endl;
    return -1;
  }
  configure_glyph_system (&gs, 20, color);

  Light light_0 = {
    .intensity = 220.f,
    .attenuation = 0.02f,
    .position = {518.f, 334.f},
    .color = {.r = 1.0f, .g = 1.0f, .b = 1.0f},
  }; 

  Light light_1 = {
    .intensity = 200.f,
    .attenuation = 0.01f,
    .position = {318.f, 337.f},
    .color = {.r = 1.0f, .g = 0.f, .b = 0.1f},
  };

  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  //Uint32 shadow_color = SDL_MapRGBA(format, 0, 0, 0, 255);
  SDL_FreeFormat(format);

  const std::vector<MaskObject> segments = get_segments_2();

  bool cont = true;
  SDL_Event event;
  
  Visualizer<D2FIG> img_mod = ImageModifier::chargePNG("../images/psic2.png").resize(200, 200).cast(glb);
  
  ViewMask view_0 (glb->get_width(), glb->get_height());
  ViewMask view_1 (glb->get_width(), glb->get_height());
  float aux_time_1 = 0.f, avg_time_1 = 0.f;

  while (cont) {
    glb->begin_render();
      img_mod.draw (glb, Dir2 {200.f, 200.f});

      aux_time_1 += 1;

      view_0.draw_light_uniform_mask (light_0);
      glb->apply_mask (view_0 | view_1.draw_light_uniform_mask (light_1));

      float a = glb->get_time();
      avg_time_1 += (a - avg_time_1) / aux_time_1;

      SDL_SetRenderDrawColor(glb->get_render(), 255, 255, 255, 255);
      for (const auto& segment: segments) {
        SDL_RenderDrawLine(glb->get_render(), segment.point1.x(), segment.point1.y(), segment.point2.x(), segment.point2.y());
      }

      SDL_SetRenderDrawColor(glb->get_render(), 0, 255, 0, 255);
      SDL_RenderDrawPoint(glb->get_render(), light_0.position.x(), light_0.position.y());

      std::string aux_str_1 = "light_0: (" + 
        std::to_string(light_0.position.x()) + ", " + 
        std::to_string(light_0.position.y()) + ")";
      std::string aux_str_2 = "light_1: (" + 
        std::to_string(light_1.position.x()) + ", " + 
        std::to_string(light_1.position.y()) + ")";
      std::string aux_str_3 = "tiempo: " + std::to_string(a) + ", avg: " + std::to_string(avg_time_1);
      gs.print (aux_str_1, 20, color, Dir2(width - 300.f, 20.f));
      gs.print (aux_str_2, 20, color, Dir2(width - 300.f, 50.f));
      gs.print (aux_str_3, 20, color, Dir2(width - 300.f, 80.f));
    glb->end_render();
    
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
        
        case SDL_MOUSEMOTION:
          light_0.position.x(event.motion.x);
          light_0.position.y(event.motion.y);
          break;
      }
    }

    uint32_t delta = glb->get_ticks();
    if (delta < SCREEN_TICKS_PER_FRAME) {
      SDL_Delay (SCREEN_TICKS_PER_FRAME - delta);
    }
  } 
}
