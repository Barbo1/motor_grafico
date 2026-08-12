#include "../../headers/concepts/lights.hpp"
#include "../../headers/concepts/glyph_system.hpp"
#include "../../headers/concepts/image_modifier.hpp"
#include "../../headers/primitives/global.hpp"
#include "../../headers/primitives/arena.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <iostream>

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
 
MaskObjectList get_segments_1 (DynamicalArena& darena) {
  std::vector<std::array<MemDir2, 2>> a = {
    std::array<MemDir2, 2>({MemDir2 {134.f, 114.f}, MemDir2 {52.f, 274.f}}),
    std::array<MemDir2, 2>({MemDir2 {245.f, 103.f}, MemDir2 {604.f, 76.f}}),
    std::array<MemDir2, 2>({MemDir2 {678.f, 149.f}, MemDir2 {411.f, 178.f}}),
    std::array<MemDir2, 2>({MemDir2 {117.f, 355.f}, MemDir2 {278.f, 195.f}}),
    std::array<MemDir2, 2>({MemDir2 {531.f, 218.f}, MemDir2 {634.f, 210.f}}),
    std::array<MemDir2, 2>({MemDir2 {566.f, 441.f}, MemDir2 {382.f, 313.f}}),
    std::array<MemDir2, 2>({MemDir2 {617.f, 466.f}, MemDir2 {388.f, 437.f}}),
    std::array<MemDir2, 2>({MemDir2 {121.f, 265.f}, MemDir2 {172.f, 199.f}}),
    std::array<MemDir2, 2>({MemDir2 {464.f, 348.f}, MemDir2 {447.f, 337.f}}),
    std::array<MemDir2, 2>({MemDir2 {498.f, 370.f}, MemDir2 {506.f, 377.f}}),
    std::array<MemDir2, 2>({MemDir2 {529.f, 390.f}, MemDir2 {546.f, 407.f}}),
    std::array<MemDir2, 2>({MemDir2 {540.f, 320.f}, MemDir2 {595.f, 355.f}}),
  };

  MaskObjectList ret = {
    .obj = nullptr,
    .size = a.size()
  };

  for (auto& elem: a) {
    MaskObject* ret_new = darena.alloc_mo();
    ret_new->point1 = elem[0];
    ret_new->point2 = elem[1];
    ret_new->next = ret.obj;
    ret.obj = ret_new->next;
  }

  return ret;
}

MaskObjectList get_segments_2 (DynamicalArena& darena) {
  std::vector<std::array<MemDir2, 2>> a = {
    std::array<MemDir2, 2>({MemDir2 {122.f, 84.f}, MemDir2 {86.f, 125.f}}),
    std::array<MemDir2, 2>({MemDir2 {86.f, 125.f}, MemDir2 {53.f, 1.78f}}),
    std::array<MemDir2, 2>({MemDir2 {53.f, 1.78f}, MemDir2 {62.f, 221.f}}),
    std::array<MemDir2, 2>({MemDir2 {62.f, 221.f}, MemDir2 {121.f, 196.f}}),
    std::array<MemDir2, 2>({MemDir2 {121.f, 196.f}, MemDir2{112.f, 243.f}}),
    std::array<MemDir2, 2>({MemDir2 {112.f, 243.f}, MemDir2{20.f, 302.f}}),
    std::array<MemDir2, 2>({MemDir2 {20.f, 302.f}, MemDir2 {50.f, 360.f}}),
    std::array<MemDir2, 2>({MemDir2 {50.f, 360.f}, MemDir2 {106.f, 330.f}}),
    std::array<MemDir2, 2>({MemDir2 {106.f, 330.f}, MemDir2{60.f, 315.f}}),
    std::array<MemDir2, 2>({MemDir2 {60.f, 315.f}, MemDir2 {108.f, 293.f}}),
    std::array<MemDir2, 2>({MemDir2 {108.f, 293.f}, MemDir2{164.f, 311.f}}),
    std::array<MemDir2, 2>({MemDir2 {164.f, 311.f}, MemDir2{110.f, 378.f}}),
    std::array<MemDir2, 2>({MemDir2 {110.f, 378.f}, MemDir2{40.f, 429.f}}),
    std::array<MemDir2, 2>({MemDir2 {40.f, 429.f}, MemDir2 {37.f, 470.f}}),
    std::array<MemDir2, 2>({MemDir2 {37.f, 470.f}, MemDir2 {127.f, 502.f}}),
    std::array<MemDir2, 2>({MemDir2 {127.f, 502.f}, MemDir2{330.f, 468.f}}),
    std::array<MemDir2, 2>({MemDir2 {330.f, 468.f}, MemDir2{157.f, 438.f}}),
    std::array<MemDir2, 2>({MemDir2 {157.f, 438.f}, MemDir2{282.f, 357.f}}),
    std::array<MemDir2, 2>({MemDir2 {282.f, 357.f}, MemDir2{546.f, 482.f}}),
    std::array<MemDir2, 2>({MemDir2 {546.f, 482.f}, MemDir2{765.f, 473.f}}),
    std::array<MemDir2, 2>({MemDir2 {765.f, 473.f}, MemDir2{750.f, 245.f}}),
    std::array<MemDir2, 2>({MemDir2 {750.f, 245.f}, MemDir2{703.f, 284.f}}),
    std::array<MemDir2, 2>({MemDir2 {703.f, 284.f}, MemDir2{500.f, 200.f}}),
    std::array<MemDir2, 2>({MemDir2 {500.f, 200.f}, MemDir2{532.f, 287.f}}),
    std::array<MemDir2, 2>({MemDir2 {532.f, 287.f}, MemDir2{409.f, 230.f}}),
    std::array<MemDir2, 2>({MemDir2 {409.f, 230.f}, MemDir2{446.f, 104.f}}),
    std::array<MemDir2, 2>({MemDir2 {446.f, 104.f}, MemDir2{269.f, 139.f}}),
    std::array<MemDir2, 2>({MemDir2 {269.f, 139.f}, MemDir2{250.f, 117.f}}),
    std::array<MemDir2, 2>({MemDir2 {250.f, 117.f}, MemDir2{239.f, 85.f}}),
    std::array<MemDir2, 2>({MemDir2 {239.f, 85.f}, MemDir2 {183.f, 135.f}}),
    std::array<MemDir2, 2>({MemDir2 {183.f, 135.f}, MemDir2{280.f, 257.f}}),
    std::array<MemDir2, 2>({MemDir2 {280.f, 257.f}, MemDir2{173.f, 196.f}}),
    std::array<MemDir2, 2>({MemDir2 {173.f, 196.f}, MemDir2{122.f, 84.f}}),
    std::array<MemDir2, 2>({MemDir2 {364.f, 307.f}, MemDir2{531.f, 333.f}}),
    std::array<MemDir2, 2>({MemDir2 {531.f, 333.f}, MemDir2{655.f, 331.f}}),
    std::array<MemDir2, 2>({MemDir2 {655.f, 331.f}, MemDir2{730.f, 379.f}}),
    std::array<MemDir2, 2>({MemDir2 {730.f, 379.f}, MemDir2{718.f, 438.f}}),
    std::array<MemDir2, 2>({MemDir2 {718.f, 438.f}, MemDir2{572.f, 436.f}}),
    std::array<MemDir2, 2>({MemDir2 {572.f, 436.f}, MemDir2{364.f, 307.f}}),
    std::array<MemDir2, 2>({MemDir2 {554.f, 49.f}, MemDir2 {646.f, 82.f}}),
    std::array<MemDir2, 2>({MemDir2 {646.f, 82.f}, MemDir2 {681.f, 163.f}})
  };

  MaskObjectList ret = {
    .obj = nullptr,
    .size = a.size()
  };

  for (const auto& as: a) {
    MaskObject* ret_new = darena.alloc_mo();
    ret_new->point1 = as[0];
    ret_new->point2 = as[1];
    ret_new->next = ret.obj;
    ret.obj = ret_new;
  }

  return ret;
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
  uint32_t height = glb->get_height();
  SDL_Rect screen_metrics = SDL_Rect {
    .x = static_cast<int>(width) / 2,
    .y = static_cast<int>(height) / 2,
    .w = static_cast<int>(width),
    .h = static_cast<int>(height)
  };
  Arena arena = Arena(1000*1000*8);
  DynamicalArena darena = DynamicalArena(10000*8);
  
  int32_t error;
  SDL_Color color = SDL_Color {255,255,255,255};
  GlyphsSystem gs (glb, &arena, "../fuentes_letras/Nostard-Medium.ttf", &error);
  if (error < 0) {
    std::cout << "problema al cargar fuentes de letra." << std::endl;
    return -1;
  }
  configure_glyph_system (&gs, 20, color);

  Light light_0 = {
    .intensity = 70.f,
    .attenuation = 0.02f,
    .position = {518.f, 334.f},
    .color = {.r = 1.0f, .g = 1.0f, .b = 1.0f},
  }; 

  Light light_1 = {
    .intensity = 150.f,
    .attenuation = 0.01f,
    .position = {318.f, 337.f},
    .color = {.r = 1.0f, .g = 0.f, .b = 0.1f},
  };

  Light light_2 = {
    .intensity = 150.f,
    .attenuation = 0.01f,
    .position = {400.f, 400.f},
    .color = {.r = 1.0f, .g = 1.0f, .b = 0.1f},
  };

  MaskObjectList segments = get_segments_2(darena);

  bool cont = true;
  SDL_Event event;
  
  Visualizer<D2FIG> img_mod = ImageModifier::chargePNG("../images/psic2.png").resize(200, 200).cast(glb);
  /*
ViewMask& ViewMask::draw_light_view_mask (
  const Light& light, 
  DynamicalArena& darena,
  const SDL_Rect& screem_metrics, 
  MaskObjectList segments
) {
   * */
  
  ViewMask view_0 (glb->get_width(), glb->get_height());
  ViewMask view_1 (glb->get_width(), glb->get_height());
  float aux_time_1 = 0.f, avg_time_1 = 0.f;
  view_1.draw_light_view_mask (light_1, darena, screen_metrics, segments);
  view_1 | view_0.draw_light_view_mask (light_2, darena, screen_metrics, segments);

  while (cont) {
    glb->begin_render();
      img_mod.draw (glb, Dir2 {200.f, 200.f});

      aux_time_1 += 1;

      view_0.draw_light_view_mask (light_0, darena, screen_metrics, segments);
      glb->apply_mask (view_0 | view_1);

      float a = glb->get_time();
      avg_time_1 += (a - avg_time_1) / aux_time_1;

      SDL_SetRenderDrawColor(glb->get_render(), 255, 255, 255, 255);
      for (MaskObject* iter = segments.obj; iter != nullptr; iter = iter->next) {
        SDL_RenderDrawLine(glb->get_render(), iter->point1.x, iter->point1.y, iter->point2.x, iter->point2.y);
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
