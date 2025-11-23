#include "../../../../headers/concepts/visualizer.hpp"
#include "../../../../headers/primitives/operations.hpp"
#include "../../../../headers/primitives/vectors.hpp"
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
  const float denom_control = v23.pL(-v31);
  if (denom_control == 0)
    return;
  const float denom = 1 / denom_control;
 
  const Dir2 dimsf = Dir2 {static_cast<float>(texture->w - 1), static_cast<float>(texture->h - 1)};
  const Dir2 diff_coefs_x = (uv1 * v23.y + uv2 * v31.y) * denom;
  const Dir2 diff_coefs_y = (uv1 * v23.x + uv2 * v31.x) * denom;

  int n, i, offset;
  float start, end, ms, me, f, s;
  SDL_Color color;
  Dir2 coefs, res;
        
  const float m3 = v31.x / v31.y;
  const float top = point2.x;
  const float bot = point3.x + v23.y * m3;

  SDL_SetRenderDrawBlendMode (glb->get_render(), SDL_BLENDMODE_NONE);

  if (v12.y != 0) {
    const float m1 = v12.x / v12.y;
    ms = min (m3, m1);
    me = max (m3, m1);
    end = start = point1.x;

    /* first iteration of uv mapping coordenates. */
    s = floor (point1.x);
    float n_f = floor (point1.y);
    res = Dir2 {s, n_f} - point3;
    coefs = uv3 - (uv1 * (res * v23L) + uv2 * (res * v31L)) * denom;

    for (n = n_f; n < floor (point2.y); n++) {
      f = ceil (end);
      for (i = s; i < f; i++) {
        res = coefs.bound01().dir_mul(dimsf);
        offset = static_cast<int>(res.x) + static_cast<int>(res.y) * texture->w;

        SDL_GetRGBA (
          *((Uint32*)texture->pixels + offset), texture->format, 
          &color.r, &color.g, &color.b, &color.a
        );
        SDL_SetRenderDrawColor (glb->get_render(), color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint (glb->get_render(), i, n);

        coefs += diff_coefs_x;
      }
      start += ms;
      end += me;

      s = floor (start);
      coefs -= diff_coefs_y + diff_coefs_x * (f - s);
    }
  }
  
  if (v23.y != 0) {
    const float m2 = v23.x / v23.y;
    ms = max (m3, m2);
    me = min (m3, m2);
    max_min (m2, m3, top, bot, &start, &end);

    s = floor (start);
    float n_f = floor (point2.y);
    res = Dir2 {s, n_f} - point3;
    coefs = uv3 - (uv1 * (res * v23L) + uv2 * (res * v31L)) * denom;

    for (n = n_f; n < floor (point3.y); n++) {
      f = ceil (end);
      for (i = s; i < f; i++) {
        res = coefs.bound01().dir_mul(dimsf);
        offset = static_cast<int>(res.x) + static_cast<int>(res.y) * texture->w;

        SDL_GetRGBA (
          *((Uint32*)texture->pixels + offset), texture->format, 
          &color.r, &color.g, &color.b, &color.a
        );
        SDL_SetRenderDrawColor (glb->get_render(), color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint (glb->get_render(), i, n);

        coefs += diff_coefs_x;
      }
      start += ms;
      end += me;

      s = floor (start);
      coefs -= diff_coefs_y + diff_coefs_x * (f - s);
    }
  }
}
