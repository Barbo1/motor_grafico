#include "../../../../headers/concepts/texture.hpp"
#include <cstdint>

Texture Texture::triangle (SDL_Renderer* render, Dir2 point1, Dir2 point2, Dir2 point3, SDL_Color color) {
  if (point2.y < point1.y) std::swap (point2, point1);
  if (point3.y < point2.y) std::swap (point3, point2);
  if (point2.y < point1.y) std::swap (point2, point1);

  Dir2 max = point1;
  Dir2 min = point1;
  if (max.x < point2.x) max.x = point2.x;
  else min.x = point2.x;
  if (max.x < point3.x) max.x = point3.x;
  if (min.x > point3.x) min.x = point3.x;

  max.y = point3.y;

  max -= min;
  point1 -= min;
  point2 -= min;
  point3 -= min;

  SDL_Point center;
  Dir2 pre_center = (point1 + point2 + point3) * (1.f/3.f);
  center.x = pre_center.x;
  center.y = pre_center.y;

  float m1 = (point1.x - point2.x) / (point1.y - point2.y);
  float c1 = point2.x - point2.y * m1;

  float m2 = (point2.x - point3.x) / (point2.y - point3.y);
  float c2 = point3.x - point3.y * m2;

  float m3 = (point1.x - point3.x) / (point1.y - point3.y);
  float c3 = point3.x - point3.y * m3;

  float m4 = m3;
  float c4 = c3;

  if (m3 < m2) {
    std::swap (m1, m3);
    std::swap (c1, c3);
    std::swap (m2, m4);
    std::swap (c2, c4);
  }

  int width = static_cast<int>(max.x);
  int height = static_cast<int>(max.y);

  std::vector<int> bounds;
  bounds.resize (2 * height + 1);
  std::vector<uint32_t>::iterator biter;

  float x = 0;
  int n = 0;
  int r = 0;
  for (; n < std::lround (point2.y); x += 1, n++, r += width) {
    *(biter++) = std::lround (m3*x + c3) + r;
    *(biter++) = std::lround (m1*x + c1) + r;
  }
  for (; n < std::lround (point3.y); x += 1, n++, r += width) {
    *(biter++) = std::lround (m4*x + c4) + r;
    *(biter++) = std::lround (m2*x + c2) + r;
  }

  *biter = height * width + 1;

  Texture ret = bounder(render, bounds.data(), height, width, color, center);
  return ret;
}
