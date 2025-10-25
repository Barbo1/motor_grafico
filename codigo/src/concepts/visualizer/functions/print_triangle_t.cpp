#include "../../../../headers/concepts/visualizer.hpp"
#include "../../../../headers/primitives/operations.hpp"
#include <SDL2/SDL_render.h>

void print_triangle_t (
  Global* glb, Dir2 point1, Dir2 point2, Dir2 point3, 
  Dir2 uv1, Dir2 uv2, Dir2 uv3, SDL_Surface* texture
) {
  if (point2.y < point1.y) { 
    std::swap (point2, point1);
    std::swap (uv2, uv1);
  }
  if (point3.y < point2.y) { 
    std::swap (point3, point2);
    std::swap (uv3, uv2);
  }
  if (point2.y < point1.y) { 
    std::swap (point2, point1);
    std::swap (uv2, uv1);
  }

  uv1 -= uv3;
  uv2 -= uv3;

  const Dir2 v12 = point1 - point2;
  const Dir2 v23 = point2 - point3;
  const Dir2 v31 = point3 - point1;
  const Dir2 v23L = v23.percan();
  const Dir2 v31L = v31.percan();
  const float denom_control = v23 * v31L;
  if (denom_control == 0)
    return;
  const float denom = 1 / denom_control;
  
  const float tex_w = static_cast<float>(texture->w - 1);
  const float tex_h = static_cast<float>(texture->h - 1);
  const Dir2 diff_coefs_x = (uv1 * v23.y + uv2 * v31.y) * denom;
  const Dir2 diff_coefs_y = (uv1 * v23.x + uv2 * v31.x) * denom;

  int n, i, offset;
  float start, end, ms, me, n_f, i_f, q;
  SDL_Color color;
  Dir2 coefs, res;
        
  const float m3 = v31.x / v31.y;
  const float top = point2.x;
  const float bot = point3.x + v23.y * m3;

  SDL_SetRenderDrawBlendMode (glb->render, SDL_BLENDMODE_NONE);

  if (v12.y != 0) {
    const float m1 = v12.x / v12.y;
    ms = min (m3, m1);
    me = max (m3, m1);
    end = start = point1.x;

    i_f = floor (point1.x);
    n_f = floor (point1.y);
    res = Dir2 {i_f, n_f} - point3;
    coefs = uv3;
    coefs += (uv1 * (res * v23L) + uv2 * (res * v31L)) * denom;

    for (n = n_f; n < floor (point2.y); n_f += 1, n++) {
      q = ceil (end);
      for (i = i_f; i < q; i++) {
        offset = 
          static_cast<int>(bound0 (coefs.x, 1.f) * tex_w) + 
          static_cast<int>(bound0 (coefs.y, 1.f) * tex_h) * texture->w;

        SDL_GetRGBA (
          *((Uint32*)texture->pixels + offset), texture->format, 
          &color.r, &color.g, &color.b, &color.a
        );
        SDL_SetRenderDrawColor (glb->render, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint (glb->render, i, n);

        coefs += diff_coefs_x;
      }
      start += ms;
      end += me;

      i_f = floor (start);
      coefs -= diff_coefs_y;
      coefs -= diff_coefs_x * (q - i_f);
    }
  }
  
  if (v23.y != 0) {
    const float m2 = v23.x / v23.y;
    ms = max (m3, m2);
    me = min (m3, m2);
    max_min (m2, m3, top, bot, &start, &end);

    i_f = floor (start);
    n_f = floor (point2.y);
    res = Dir2 {i_f, n_f} - point3;
    coefs = uv3;
    coefs += (uv1 * (res * v23L) + uv2 * (res * v31L)) * denom;

    for (n = n_f; n_f < floor (point3.y); n_f += 1, n++) {
      q = ceil (end);
      for (i = i_f; i < q; i++) {
        offset = 
          static_cast<int>(bound0 (coefs.x, 1.f) * tex_w) + 
          static_cast<int>(bound0 (coefs.y, 1.f) * tex_h) * texture->w;

        SDL_GetRGBA (
          *((Uint32*)texture->pixels + offset), texture->format, 
          &color.r, &color.g, &color.b, &color.a
        );
        SDL_SetRenderDrawColor (glb->render, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint (glb->render, i, n);

        coefs += diff_coefs_x;
      }
      start += ms;
      end += me;

      i_f = floor (start);
      coefs -= diff_coefs_y;
      coefs -= diff_coefs_x * (q - i_f);
    }
  }
}
