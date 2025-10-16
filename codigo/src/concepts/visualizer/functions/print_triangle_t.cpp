#include "../../../../headers/concepts/visualizer.hpp"
#include "../../../../headers/primitives/operations.hpp"
#include <utility>
#include <algorithm>

SDL_Color deduce_pixel_color (
  Dir2&& v2, const Dir2& point1, const Dir2& point2, const Dir2& point3, 
  const Dir2& uv1, const Dir2& uv2, const Dir2& uv3, const SDL_Surface* texture, 
  const float & tex_w, const float & tex_h, const Dir2& v0, const Dir2& v1, 
  const float& d00, const float& d01, const float& d11
) {
  v2 -= point1;
  float d20 = v2 * v0;
  float d21 = v2 * v1;

  float lam2 = d11 * d20 - d01 * d21;
  float lam3 = d00 * d21 - d01 * d20;

  Dir2 coefs = uv1 + uv2 * lam2 + uv3 * lam3;
  
  int offset = static_cast<int>(bound0 (coefs.x, 1.f) * tex_w) + 
    static_cast<int>(bound0 (coefs.y, 1.f) * tex_h) * texture->w;
  Uint32 pos = *((Uint32*)texture->pixels + offset);

  SDL_Color color;
  SDL_GetRGBA(pos, texture->format, &color.r, &color.g, &color.b, &color.a);

  return color;
}

void print_triangle_t (
  SDL_Renderer* render, Dir2 point1, Dir2 point2, Dir2 point3, 
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
  
  /* Precalculated values. */
  const Dir2 v0 = point2 - point1;
  const Dir2 v1 = point3 - point1;
  float d00 = v0 * v0;
  float d01 = v0 * v1;
  float d11 = v1 * v1;
  const float tex_w = static_cast<float>(texture->w - 1);
  const float tex_h = static_cast<float>(texture->h - 1);

  uv2 -= uv1;
  uv3 -= uv1;

  float denom = d00 * d11 - d01 * d01;
  if (denom == 0.0f)
    return;
  denom = 1 / denom;
  d00 *= denom;
  d01 *= denom;
  d11 *= denom;
  /* --------------------- */

  const float d23 = point2.y - point3.y;
  const float m3 = v1.x / v1.y;
    
  int n, i;
  float start, end, ms, me, n_f, i_f;
  SDL_Color color;

  const float top = point2.x;
  const float bot = point3.x + d23 * m3;
        
  SDL_SetRenderDrawBlendMode (render, SDL_BLENDMODE_NONE);

  if (v0.y != 0) {
    const float m1 = (point2.x - point1.x) / v0.y;
    ms = min (m3, m1);
    me = max (m3, m1);
    end = start = point1.x;
    for (n_f = floor (point1.y), n = n_f; n_f < floor (point2.y); n_f += 1, n++) {
      for (i_f = floor (start), i = i_f; i_f <= ceil (end); i_f += 1, i++) {
        color = deduce_pixel_color (
          Dir2{i_f, n_f}, point1, point2, point3, uv1, uv2, uv3, 
          texture, tex_w, tex_h, v0, v1, d00, d01, d11
        );
        SDL_SetRenderDrawColor (render, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint (render, i, n);
      }
      start += ms;
      end += me;
    }
  }
  
  if (d23 != 0) {
    const float m2 = (point2.x - point3.x) / d23;
    ms = max (m3, m2);
    me = min (m3, m2);
    max_min (m2, m3, top, bot, &start, &end);
    for (n_f = floor (point2.y), n = n_f; n_f < floor (point3.y); n_f += 1, n++) {
      for (i_f = floor (start), i = i_f; i_f <= ceil (end); i_f += 1, i++) {
        color = deduce_pixel_color (
          Dir2{i_f, n_f}, point1, point2, point3, uv1, uv2, uv3, 
          texture, tex_w, tex_h, v0, v1, d00, d01, d11
        );
        SDL_SetRenderDrawColor (render, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint (render, i, n);
      }
      start += ms;
      end += me;
    }
  }
}
