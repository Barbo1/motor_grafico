#include "../../../../headers/concepts/visualizer.hpp"
#include <cmath>
#include <array>
#include <algorithm>
#include <cstdint>

void print_polygon_c (Global* glb, std::vector<Dir2> points, SDL_Color color) {
  std::size_t many_points = points.size();
    
  /* Searching maximum and minimum coordenates. */
  float min_h = points[0].y, max_h = points[0].y;
  for (auto& point: points) {
    max_h = std::fmax (max_h, point.y);
    min_h = std::fmin (min_h, point.y);
  }

  /* Calculating coefitients. */
  std::vector<std::array<double, 4>> coefs = std::vector<std::array<double, 4>>(many_points, {0.f, 0.f, 0.f, 0.f});
  std::vector<bool> conds = std::vector<bool>(many_points, false);
  points.push_back (points[0]);
  points.push_back (points[1]);
  for (std::size_t i = 1; i <= many_points; i++) {
    if (points[i+1].y != points[i].y) {
      double denom = 1 / (points[i].y - points[i+1].y);
      double mi = (points[i].x - points[i+1].x) * denom;
      double ci = points[i+1].x - points[i+1].y * mi;
      double q = -denom;
      double p = -points[i].y * q;
      coefs[i-1] = {mi, mi*min_h + ci, q, q*min_h + p};
      conds[i-1] = (points[i].y - points[i-1].y) * (points[i].y - points[i+1].y) < 0;
    }
  }

  /* Calculate the bound to make the texture. */
  static std::vector<int> bounds;
  static std::vector<int>::iterator biter;
  static std::vector<int>::iterator piter;

  SDL_SetRenderDrawColor(glb->get_render(), color.r, color.g, color.b, color.a);
  SDL_SetRenderDrawBlendMode(glb->get_render(), SDL_BLENDMODE_ADD);

  bounds.reserve (many_points);
  for (uint32_t level = std::roundl (min_h); level <= std::roundl (max_h); level++) {
    for (uint32_t i = 0; i < coefs.size(); i++) {
      auto& [d_elem, elem, d_q, q] = coefs[i];

      if (-0.00001 < q && q < 1.00001) [[likely]] {
        int32_t inter = std::lround (elem);
        if (!conds[i] || std::ranges::all_of (
          bounds.begin (), bounds.end (), [&] (int elem) {return elem != inter;}
        )) [[likely]] bounds.push_back (inter);
      }

      q += d_q; elem += d_elem;
    }
    for (piter = bounds.begin (); piter != bounds.end(); piter++)
      for (biter = piter + 1; biter != bounds.end(); biter++)
        if (*biter < *piter)
          std::swap (*piter, *biter); 
    for (uint32_t i = 0; i < (bounds.size() & ~(uint64_t)1); i += 2)
      for (int offset = bounds[i]; offset <= bounds[i+1]; offset++)
        SDL_RenderDrawPoint (glb->get_render(), offset, level);
    bounds.clear();
  }
}
