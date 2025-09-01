#include "../../../../../headers/concepts/texture.hpp"
#include <utility>

void print_triangle_c (SDL_Renderer* render, Dir2 point1, Dir2 point2, Dir2 point3, SDL_Color color) {
  if (point2.y < point1.y) std::swap (point2, point1);
  if (point3.y < point2.y) std::swap (point3, point2);
  if (point2.y < point1.y) std::swap (point2, point1);

  const int y2 = floor (point2.y);
  const double d12 = point1.y - point2.y;
  const double d23 = point2.y - point3.y;
  const double m3 = (point1.x - point3.x) / (point1.y - point3.y);

  const double top = point2.x;
  const double bot = point3.x + d23 * m3;

  SDL_SetRenderDrawColor (render, color.r, color.g, color.b, color.a);
  SDL_SetRenderDrawBlendMode (render, SDL_BLENDMODE_ADD);

  if (d12 != 0) {
    const double m1 = (point1.x - point2.x) / d12;
    double start, end, ms, me;

    if (m3 > m1) {
      ms = m1;
      me = m3;
    } else {
      ms = m3;
      me = m1;
    }

    end = start = point1.x;
    for (int n = floor (point1.y); n < y2; n++) {
      for (int i = floor (start); i <= ceil (end); i++)
        SDL_RenderDrawPoint (render, i, n);
      start += ms;
      end += me;
    }
  }
  
  if (d23 != 0) {
    const double m2 = (point2.x - point3.x) / d23;
    double start, end, ms, me;

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

    for (int n = y2; n < floor (point3.y); n++) {
      for (int i = floor (start); i <= ceil (end); i++)
        SDL_RenderDrawPoint (render, i, n);
      start += ms;
      end += me;
    }
  }
}
