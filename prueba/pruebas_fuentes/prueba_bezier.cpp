#include "../../headers/primitives/global.hpp"
#include "../../headers/primitives/file_processing.hpp"
#include "../../headers/concepts/image_modifier.hpp"
#include "../../headers/concepts/visualizer.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <iostream>
#include <cmath>
#include <smmintrin.h>
#include <string>
#include <vector>


int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});

  bool cont = true;
  SDL_Event event;

  Dir2 P1 = Dir2 (343.4f, 396.1f);
  Dir2 P2 = Dir2 (233.3f, 585.2f);
  Dir2 P3 = Dir2 (276.1f, 172.0f);

  Dir2 av = P1 - P2 * 2.f + P3;
  Dir2 bv = P1 - P2;
  float rem_sqrt = P2.y * P2.y - P1.y * P3.y;
  float iavy = _mm_cvtss_f32 ( _mm_permute_ps (_mm_rcp_ps (av.v), 0b01010101));

  float bottom = _mm_cvtss_f32 ( _mm_permute_ps (_mm_min_ps (_mm_min_ps (P1.v, P2.v), P3.v), 0b01010101));
  float top = _mm_cvtss_f32 ( _mm_permute_ps (_mm_max_ps (_mm_max_ps (P1.v, P2.v), P3.v), 0b01010101));

  while (cont) {
    SDL_Delay(1);

    glb->begin_render();

      SDL_SetRenderDrawColor(glb->get_render(), 255, 0, 0, 255);
      float y_d = bottom;

      float prev_t;
      while (y_d < top && (prev_t = std::fmaf (av.y, y_d, rem_sqrt)) < 0)
        y_d += 1.f;

      while (y_d < top && (prev_t = std::fmaf (av.y, y_d, rem_sqrt)) > 0) {
        float sqrt_prev_t = std::sqrt (prev_t);
        float t1 = iavy * (bv.y + sqrt_prev_t);
        float t2 = iavy * (bv.y - sqrt_prev_t);
        float x1 = P1.x - 2 * bv.x * t1 + av.x * t1 * t1;
        float x2 = P1.x - 2 * bv.x * t2 + av.x * t2 * t2;
        
        if (t1 > 0.f && t1 < 1.f)
          SDL_RenderDrawPoint(glb->get_render(), static_cast<uint32_t>(x1), static_cast<uint32_t>(y_d));
        if (t2 > 0.f && t2 < 1.f)
          SDL_RenderDrawPoint(glb->get_render(), static_cast<uint32_t>(x2), static_cast<uint32_t>(y_d));

        y_d += 1.f;
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

    glb->end_render();
  } 
}
