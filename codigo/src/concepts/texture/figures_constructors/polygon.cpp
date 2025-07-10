#include "../../../../headers/concepts/texture.hpp"
    
Texture Texture::polygon (SDL_Renderer* render, std::vector<Dir2> points, SDL_Color color) {
  std::size_t many_points = points.size();
  if (many_points > 64) return Texture();
    
  /* Searching maximum and minimum coordenates. */
  Dir2 max(points[0]), min(points[0]);
  for (auto& point: points) {
    if (max.x < point.x) max.x = point.x;
    if (min.x > point.x) min.x = point.x;
    if (max.y < point.y) max.y = point.y;
    if (min.y > point.y) min.y = point.y;
  }

  /* Calculating texture parameter. */
  int height = max.y - min.y;
  int width = max.x - min.x;
  Dir2 center;
  for (auto& point: points) {
    point -= min;
    center += point;
  }
  center *= (1.f/many_points);

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
      {{mi, ci, q, p}, (points[i].y - points[i-1].y) * (points[i].y - points[i+1].y) < 0}
    );
  }

  /* Sorting the lines(optional). */

  /* Calculate the bound to make the texture. */
  int * bounds = new int[height * many_points];
  int * biter = bounds;
  int * piter = bounds;
  int * liter;

  for (uint32_t level = 0; level < width * height; level += width) {
    for (auto & mc: coef) {
      double &q = mc.first[3], &elem = mc.first[1];
      if (-0.00001 < q && q < 1.00001) {
        int32_t inter = std::lround(elem) + level;
        if (mc.second) {
          liter = piter;
          while (liter < biter && *liter != inter) liter++;
          if (liter < biter) goto FIN; 
        }
        *(biter++) = inter;
      }
FIN:
      q += mc.first[2]; elem += mc.first[0];
    }
    for (; piter < biter; piter++)
      for (liter = piter + 1; liter < biter; liter++)
        if (*liter < *piter)
          std::swap (*piter, *liter); 
  }
  *biter = height * width;
  
  Texture ret = bounder(render, bounds, height, width, color, {static_cast<int>(center.x), static_cast<int>(center.y)});
  delete [] bounds;
  return ret; 
}

/* Sorting
  std::array<uint64_t, 65> order{0};
  auto calc_colition_forth = [&](std::size_t i, std::size_t j) {
    const double r = coef[i].first[0] * points[j].y + coef[i].first[1];
    return r*(r - points[j].x) > 0 && (points[j].y - points[i].y)*(points[i+1].y - points[j].y) > 0;
  };
  auto calc_colition_back = [&](std::size_t i, std::size_t j) {
    const double r = coef[i].first[0] * points[j].y + coef[i].first[1];
    return r*(r - points[j].x) < 0 && (points[j].y - points[i].y)*(points[i+1].y - points[j].y) > 0;
  };
  for (std::size_t i = 0; i < many_points; i++) {
    for (std::size_t j = 0; j < many_points; j++) {
      if (j != i) {
        bool res = 
          calc_colition_back (i, j+1) || 
          calc_colition_back (i, j) ||  
          calc_colition_forth (j, i+1) || 
          calc_colition_forth (j, i);
        order[i] |= res << j;
      }
    }
  }
  order[0] |= order[many_points];
  uint64_t already = 0, end = (1 << many_points) - 1;
  std::vector<coefType> ordered_coef;
  ordered_coef.resize (many_points);
  std::vector<coefType>::iterator iter = ordered_coef.begin ();

  std::cout << "elementos: " << std::endl;
  for (int i = 0; i < many_points; i++) {
    std::cout << "\telem " << i << ": " << std::bitset<16>(order[i]) << std::endl;
  }
  int64_t prev = 1;
  while (already != end) {
    if (prev - (int64_t)already != 0) {
      std::cout << "already: " << std::bitset<16>(already) << std::endl;
      prev = already;
    }
    for (std::size_t j = 0; j < many_points; j++) {
      const uint64_t& pot = powers[j];
      if ((pot & already) == 0 && (order[j] & ~already) == 0) {
        already |= pot;
        *(iter++) = coef[j];
      }
    }
  }
*/
