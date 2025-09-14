#include "../../../../../headers/concepts/visualizer.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <utility>
#include <algorithm>

void print_triangle_c (SDL_Renderer* render, Dir2 point1, Dir2 point2, Dir2 point3, SDL_Color color) {
  if (point2.y < point1.y) std::swap (point2, point1);
  if (point3.y < point2.y) std::swap (point3, point2);
  if (point2.y < point1.y) std::swap (point2, point1);

  const double d12 = point1.y - point2.y;
  const double d23 = point2.y - point3.y;
  const double m3 = (point1.x - point3.x) / (point1.y - point3.y);
    
  double start, end, ms, me;

  const double top = point2.x;
  const double bot = point3.x + d23 * m3;

  SDL_SetRenderDrawColor (render, color.r, color.g, color.b, color.a);
  SDL_SetRenderDrawBlendMode (render, SDL_BLENDMODE_ADD);

  if (d12 != 0) {
    const double m1 = (point1.x - point2.x) / d12;

    if (m3 > m1) {
      ms = m1;
      me = m3;
    } else {
      ms = m3;
      me = m1;
    }

    end = start = point1.x;
    for (int n = floor (point1.y); n < floor (point2.y); n++) {
      for (int i = floor (start); i <= ceil (end); i++)
        SDL_RenderDrawPoint (render, i, n);
      start += ms;
      end += me;
    }
  }
  
  if (d23 != 0) {
    const double m2 = (point2.x - point3.x) / d23;

    if (m3 < m2) {
      ms = m2;
      me = m3;
      start = top;
      end = bot;
    } else {
      ms = m3;
      me = m2;
      start = bot;
      end = top;
    }

    for (int n = floor (point2.y); n < floor (point3.y); n++) {
      for (int i = floor (start); i <= ceil (end); i++)
        SDL_RenderDrawPoint (render, i, n);
      start += ms;
      end += me;
    }
  }
}

SDL_Color deduce_pixel_color (const float x, const float y, 
  const Dir2& point1, const Dir2& point2, const Dir2& point3, const Dir2& uv1, 
  const Dir2& uv2, const Dir2& uv3, const SDL_Surface* texture,
  const float & tex_w, const float & tex_h,
  const Dir2& v0, const Dir2& v1,
  const float& d00, const float& d01, const float& d11, const float& denom
) {
  Dir2 v2 = Dir2 {x, y};
  v2 -= point1;

  float d20 = v2 * v0;
  float d21 = v2 * v1;

  float lam2 = (d11 * d20 - d01 * d21) * denom;
  float lam3 = (d00 * d21 - d01 * d20) * denom;
  float lam1 = 1.0f - lam2 - lam3;

  float coefx = uv1.x * lam1 + uv2.x * lam2 + uv3.x * lam3;
  coefx = std::min(coefx, 1.0f);
  coefx = std::max(coefx, 0.0f);

  float coefy = uv1.y * lam1 + uv2.y * lam2 + uv3.y * lam3;
  coefy = std::min(coefy, 1.0f);
  coefy = std::max(coefy, 0.0f);
  
  int offset = static_cast<int>(coefx * tex_w);
  offset += static_cast<int>(coefy * tex_h) * texture->w;
  Uint32 pos = *((Uint32*)texture->pixels + offset);

  SDL_Color color;
  SDL_GetRGBA(pos, texture->format, &color.r, &color.g, &color.b, &color.a);

  return color;
}

void print_triangle_t (
  SDL_Renderer* render, 
  Dir2 point1, Dir2 point2, Dir2 point3, 
  Dir2 uv1, Dir2 uv2, Dir2 uv3, 
  SDL_Surface* texture
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
  const float d00 = v0 * v0;
  const float d01 = v0 * v1;
  const float d11 = v1 * v1;
  const float tex_w = static_cast<float>(texture->w - 1);
  const float tex_h = static_cast<float>(texture->h - 1);

  float denom = d00 * d11 - d01 * d01;
  if (denom == 0.0f)
    return;
  denom = 1 / denom;
  /* --------------------- */

  const double d12 = point1.y - point2.y;
  const double d23 = point2.y - point3.y;
  const double m3 = (point1.x - point3.x) / (point1.y - point3.y);
    
  double start, end, ms, me;
  SDL_Color color;

  const double top = point2.x;
  const double bot = point3.x + d23 * m3;
        
  SDL_SetRenderDrawBlendMode (render, SDL_BLENDMODE_NONE);

  if (d12 != 0) {
    const double m1 = (point1.x - point2.x) / d12;

    if (m3 > m1) {
      ms = m1;
      me = m3;
    } else {
      ms = m3;
      me = m1;
    }

    end = start = point1.x;
    for (int n = floor (point1.y); n < floor (point2.y); n++) {
      for (int i = floor (start); i <= ceil (end); i++) {
        color = deduce_pixel_color (
          i, n, point1, point2, point3, uv1, uv2, uv3, 
          texture, tex_w, tex_h, v0, v1, d00, d01, d11,
          denom
        );
        SDL_SetRenderDrawColor (render, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint (render, i, n);
      }
      start += ms;
      end += me;
    }
  }
  
  if (d23 != 0) {
    const double m2 = (point2.x - point3.x) / d23;

    if (m3 < m2) {
      ms = m2;
      me = m3;
      start = top;
      end = bot;
    } else {
      ms = m3;
      me = m2;
      start = bot;
      end = top;
    }

    for (int n = floor (point2.y); n < floor (point3.y); n++) {
      for (int i = floor (start); i <= ceil (end); i++) {
        color = deduce_pixel_color (
          i, n, point1, point2, point3, uv1, uv2, uv3, 
          texture, tex_w, tex_h, v0, v1, d00, d01, d11,
          denom
        );
        SDL_SetRenderDrawColor (render, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint (render, i, n);
      }
      start += ms;
      end += me;
    }
  }
}
