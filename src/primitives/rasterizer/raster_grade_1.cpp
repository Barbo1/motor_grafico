#include "../../../headers/primitives/rasterizer.hpp"
#include <cmath>
#include <algorithm>

static void draw_line (BoolMatrixU& bound, Dir2 P1, Dir2 P2, float& prev_direction, float& next_direction) {
  Dir2 diff21 = P2 - P1;
  if (diff21.y != 0.f) {
    uint64_t prev_point_disapears = (diff21.y * prev_direction < 0.f ? 1ULL : 0ULL);
    uint64_t next_point_disapears = (diff21.y * next_direction < 0.f ? 1ULL : 0ULL);

    Dir2 PM = (P1 + P2) * 0.5f;
    float x_diff = diff21.x / diff21.y;
    if (P1.y > P2.y) {
      std::swap (P1, P2);
      std::swap (prev_point_disapears, next_point_disapears);
    }

    float x = P1.x;
    for (float y = P1.y; y <= PM.y; y++) {
      uint64_t xi = std::lround (x);
      uint64_t yi = std::lround (y);
      bound.change (yi, xi, (prev_point_disapears & bound(yi, xi)) ^ 1ULL);
      x += x_diff;
    }
      
    x = P2.x;
    for (float y = P2.y; y > PM.y; y--) {
      uint64_t xi = std::lround (x);
      uint64_t yi = std::lround (y);
      bound.change (yi, xi, (next_point_disapears & bound(yi, xi)) ^ 1ULL);
      x -= x_diff;
    }

  } else
    if (prev_direction * next_direction > 0.f)
      bound.unset (std::lround (P1.y), std::lround (P1.x));
}

SDL_Surface* raster_grade_1 (std::vector<Dir2> points, SDL_Color color, AntiAliasingType antialias) {
  float antialias_multiplier;
  switch (antialias) {
    case AAx2: antialias_multiplier = 2.f; break;
    case AAx4: antialias_multiplier = 4.f; break;
    case AAx8: antialias_multiplier = 8.f; break;
    case AAx16: antialias_multiplier = 16.f; break;
    default: antialias_multiplier = 1.f;
  }

  std::size_t many_points = points.size();
  for (Dir2& point: points)
    point = Dir2 (std::round (point.x), std::round (point.y));

  // Searching maximum and minimum coordenates.
  Dir2 min = points[0], max = points[0];
  for (const auto& point: points) {
    min.v = _mm_min_ps (min.v, point.v);
    max.v = _mm_max_ps (max.v, point.v);
  }

  // calculating dimensions of the image and matrix.
  Dir2 min_sized = min * antialias_multiplier;
  Dir2 dims_l = max - min;
  Dir2 dims = dims_l.madd (antialias_multiplier, Dir2{16.f, 16.f});

  // creating the matrix for the bounds.
  int32_t matrix_height = std::lround (dims.y) + 2;
  int32_t matrix_width = std::lround (dims.x) + 2;
  BoolMatrixU bound = BoolMatrixU (matrix_height, matrix_width);

  // generating array of the point of the scaled image.
  std::vector<Dir2> _points (many_points+2);
  for (std::size_t i = 0; i < many_points; i++) {
    _points[i] = points[i].msub(antialias_multiplier, min_sized) + Dir2 {1.f, 1.f};
  }
  _points[many_points] = _points[0];
  _points[many_points+1] = _points[1];

  // printing the bounds of the image in the matrix.
  Dir2 P1 = _points[many_points-1];
  Dir2 P2 = _points[0];
  Dir2 P3 = _points[1];
  float prev_direction = (P1 - _points[many_points-2]).y;
  float next_direction = (P3 - P2).y;
  for (std::size_t pos = 2; pos < many_points+2; pos++) {
    draw_line (bound, P1, P2, prev_direction, next_direction);
    prev_direction = (P2 - P1).y;
    P1 = std::exchange (P2, std::exchange (P3, _points[pos]));
    next_direction = (P3 - P2).y;
  }
 
  // creating the image filled with the data.
  const uint32_t height = std::lround(dims_l.y) + 1;
  const uint32_t width = std::lround(dims_l.x) + 1;
  SDL_Surface* surface = SDL_CreateRGBSurface (0, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  Uint32* pixels = (Uint32*)(surface->pixels);
  Uint32 pixel_color;
  uint32_t pos = 0;

  bound.fill_bounds();
  switch (antialias) {
    case AAx1:
      pixel_color = SDL_MapRGBA(surface->format, color.r, color.g, color.b, 0);
      for (uint32_t h = 0; h < height; h++) {
        for (uint32_t w = 0; w < width; w++) {
          uint64_t elem = bound(h, w);
          pixels[pos++] = pixel_color | ((elem << 8) - elem);
        }
      }
      break;
    case AAx2:
    case AAx4:
      break;
    case AAx8:
      pixel_color = SDL_MapRGBA(surface->format, color.r, color.g, color.b, 0);
      for (uint32_t h = 0; h < height; h++) {
        for (uint32_t w = 0; w < width; w++) {
          uint64_t elem = bound.number_bits(h, w);
          pixels[pos++] = pixel_color | ((elem << 2) - (elem > 0));
        }
      }
      break;
    case AAx16: 
      pixel_color = SDL_MapRGBA(surface->format, color.r, color.g, color.b, 0);
      for (uint32_t h = 0, h_2 = 0; h < height; h++, h_2+=2) {
        for (uint32_t w = 0, w_2 = 0; w < width; w++, w_2+=2) {
          uint64_t elem = bound.number_bits_quad(h_2, w_2);
          pixels[pos++] = pixel_color | (elem - (elem > 0));
        }
      }
      break;
  }

  return surface;
}
