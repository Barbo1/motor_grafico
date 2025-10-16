#include "../../../../headers/concepts/visualizer.hpp"
#include "../../../../headers/primitives/operations.hpp"

void print_triangle_c (SDL_Renderer* render, Dir2 point1, Dir2 point2, Dir2 point3, SDL_Color color) {
  if (point2.y < point1.y) std::swap (point2, point1);
  if (point3.y < point2.y) std::swap (point3, point2);
  if (point2.y < point1.y) std::swap (point2, point1);

  const float d12 = point1.y - point2.y;
  const float d23 = point2.y - point3.y;
  const float m3 = (point1.x - point3.x) / (point1.y - point3.y);
  const float top = point2.x;
  const float bot = point3.x + d23 * m3;
    
  float start, end, ms, me;

  SDL_SetRenderDrawColor (render, color.r, color.g, color.b, color.a);
  SDL_SetRenderDrawBlendMode (render, SDL_BLENDMODE_ADD);

  if (d12 != 0) {
    const float m1 = (point1.x - point2.x) / d12;
  
    ms = min (m3, m1);
    me = max (m3, m1);
    end = start = point1.x;
    for (int n = floor (point1.y); n < floor (point2.y); n++) {
      for (int i = floor (start); i <= ceil (end); i++)
        SDL_RenderDrawPoint (render, i, n);
      start += ms;
      end += me;
    }
  }
  
  if (d23 != 0) {
    const float m2 = (point2.x - point3.x) / d23;

    ms = max (m3, m2);
    me = min (m3, m2);
    max_min (m2, m3, top, bot, &start, &end);
    for (int n = floor (point2.y); n < floor (point3.y); n++) {
      for (int i = floor (start); i <= ceil (end); i++)
        SDL_RenderDrawPoint (render, i, n);
      start += ms;
      end += me;
    }
  }
}
