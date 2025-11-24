#include "../../../../headers/concepts/visualizer.hpp"

#include <cmath>

void print_triangle_c (Global* glb, Dir2 point1, Dir2 point2, Dir2 point3, SDL_Color color) {
  if (point2.y < point1.y) std::swap (point2, point1);
  if (point3.y < point2.y) std::swap (point3, point2);
  if (point2.y < point1.y) std::swap (point2, point1);

  const float d12 = point1.y - point2.y;
  const float d23 = point2.y - point3.y;
  const float m3 = (point1.x - point3.x) / (point1.y - point3.y);
  const float top = point2.x;
  const float bot = point3.x + d23 * m3;

  SDL_SetRenderDrawBlendMode (glb->get_render(), SDL_BLENDMODE_ADD);
  SDL_SetRenderDrawColor (glb->get_render(), color.r, color.g, color.b, color.a);

  if (d12 != 0) {
    const float m1 = (point1.x - point2.x) / d12;
    const float ms = std::fmin (m3, m1);
    const float me = std::fmax (m3, m1);
    
    float start = point1.x;
    float end = start;
    for (int n = floor (point1.y); n < floor (point2.y); n++) {
      for (int i = floor (start); i <= ceil (end); i++)
        SDL_RenderDrawPoint (glb->get_render(), i, n);
      start += ms;
      end += me;
    }
  }
  
  if (d23 != 0) {
    const float m2 = (point2.x - point3.x) / d23;
    const float ms = std::fmax (m3, m2);
    const float me = std::fmin (m3, m2);
    float start = (m2 > m3 ? top : bot);
    float end = (m2 < m3 ? top : bot);

    for (int n = floor (point2.y); n < floor (point3.y); n++) {
      for (int i = floor (start); i <= ceil (end); i++)
        SDL_RenderDrawPoint (glb->get_render(), i, n);
      start += ms;
      end += me;
    }
  }
}
