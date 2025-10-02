#include "../../../../headers/concepts/visualizer.hpp"
#include <cmath>
#include <array>

void print_polygon_c (SDL_Renderer* render, std::vector<Dir2> points, SDL_Color color) {
  std::size_t many_points = points.size();
    
  /* Searching maximum and minimum coordenates. */
  float min = points[0].y, max = points[0].y;
  for (auto& point: points) {
    if (max < point.y) max = point.y;
    if (min > point.y) min = point.y;
  }

  /* Calculating coefitients. */
  using coefType = std::pair<std::array<double, 4>, bool>;
  std::vector<coefType> coef = std::vector<coefType>();
  coef.reserve (many_points);
  points.push_back (points[0]);
  points.push_back (points[1]);
  for (std::size_t i = 1; i <= many_points; i++) {
    double mi = (points[i].x - points[i+1].x) / (points[i].y - points[i+1].y);
    double ci = points[i+1].x - points[i+1].y * mi;
    double q, p;
    if (points[i+1].x != points[i].x) {
      q = 1 / (points[i+1].x - points[i].x);
      p = (ci - points[i].x) * q;
      q = mi * q;
    } else {
      q = 1/ (points[i+1].y - points[i].y);
      p = -points[i].y * q;
    }
    coef.push_back (
      {{mi, mi*min + ci, q, q*min + p}, (points[i].y - points[i-1].y) * (points[i].y - points[i+1].y) < 0}
    );
  }

  /* Calculate the bound to make the texture. */
  static std::vector<int> bounds;
  bounds.reserve (many_points);
  static std::vector<int>::iterator biter;
  static std::vector<int>::iterator piter;

  SDL_SetRenderDrawColor(render, color.r, color.g, color.b, color.a);
  SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_ADD);

  for (uint32_t level = std::roundl(min); level <= std::roundl(max); level++) {
    for (auto & mc: coef) {
      double &q = mc.first[3], &elem = mc.first[1];
      if (-0.00001 < q && q < 1.00001) {
        int32_t inter = std::lround(elem);
        if (mc.second) {
          piter = bounds.begin ();
          while (piter != bounds.end() && *piter != inter) piter++;
          if (piter != bounds.end()) goto FIN; 
        }
        bounds.push_back (inter);
      }
FIN:
      q += mc.first[2]; elem += mc.first[0];
    }
    for (piter = bounds.begin (); piter != bounds.end(); piter++)
      for (biter = piter + 1; biter != bounds.end(); biter++)
        if (*biter < *piter)
          std::swap (*piter, *biter); 
    if ((bounds.size() & 1) == 1)
      bounds.pop_back();
    for (int i = 0; i < bounds.size(); i+=2)
      for (int offset = bounds[i]; offset <= bounds[i+1]; offset++)
        SDL_RenderDrawPoint (render, offset, level);
    bounds.clear();
  }
}
