#include "../../../headers/primitives/rasterizer.hpp"
#include <cmath>

static void draw_line (BoolMatrixU& bound, Dir2 P1, Dir2 P2, float& prev_derivate, float& next_derivate) {
  Dir2 diff21 = P2 - P1;
  uint64_t point_disapears = (diff21.y * prev_derivate <= 0.f ? 1ULL : 0ULL);

  if (std::abs(diff21.y) > 0.f) {
    Dir2 PM = (P1 + P2) * 0.5f;
    uint64_t mid = std::lround (PM.y);
    float x_diff = diff21.x / diff21.y;
    if (P1.y > P2.y)
      std::swap (P1, P2);

    float x = P1.x;
    uint64_t xi = static_cast<uint64_t>(P1.x);
    for (uint64_t y = P1.y; y <= mid; y++) {
      bound.change(y, xi, (point_disapears & bound(y, xi)) ^ 1ULL);
      x += x_diff;
      xi = std::lround (x);
    }
      
    x = P2.x;
    xi = static_cast<uint64_t>(P2.x);
    for (uint64_t y = P2.y; y > mid; y--) {
      bound.change(y, xi, (point_disapears & bound(y, xi)) ^ 1ULL);
      x -= x_diff;
      xi = std::lround (x);
    }

    prev_derivate = diff21.y;

  } else {
    if (prev_derivate * next_derivate > 0.f)
      bound.unset (P1.y, P1.x);
    prev_derivate = 0.f;
  }
}

SDL_Surface* raster_lines (std::vector<Dir2> points, SDL_Color color, AntiAliasingType antialias) {
  uint64_t aat;
  switch (antialias) {
    case AAx2: aat = 2; break;
    case AAx4: aat = 4; break;
    case AAx8: aat = 8; break;
    case AAx16: aat = 16; break;
    default: aat = 1;
  }

  float line_raster_multiplier = static_cast<float> (aat);
  std::size_t many_points = points.size();
    
  // Searching maximum and minimum coordenates.
  Dir2 min = points[0], max = points[0];
  for (const auto& point: points) {
    min.v = _mm_min_ps (min.v, point.v);
    max.v = _mm_max_ps (max.v, point.v);
  }

  // calculating dimensions of the image and matrix.
  Dir2 min_sized = min * line_raster_multiplier;
  Dir2 dims_l = (max - min);
  Dir2 dims = dims_l.madd(line_raster_multiplier, Dir2{16.f, 16.f});

  // creating the matrix for the bounds.
  int32_t matrix_height = std::lround (dims.y);
  int32_t matrix_width = std::lround (dims.x);
  BoolMatrixU B = BoolMatrixU (matrix_height, matrix_width);

  // generating array of the point of the scaled image.
  std::vector<Dir2> _points (many_points+2);
  for (std::size_t i = 0; i < many_points; i++) {
    _points[i] = points[i].msub(line_raster_multiplier, min_sized);
    _points[i] = Dir2 (std::round (_points[i].x), std::round (_points[i].y));
  }
  _points[many_points] = _points[0];
  _points[many_points+1] = _points[1];

  // printing the bounds of the image in the matrix.
  Dir2 P1 = _points[many_points-1];
  Dir2 P2 = _points[0];
  Dir2 P3 = _points[1];
  float prev_derivate = (P1 - _points[many_points-2]).y;
  float next_derivate = (P3 - P2).y;
  for (std::size_t pos = 2; pos < many_points+2; pos++) {
    draw_line (B, P1, P2, prev_derivate, next_derivate);
    P1 = P2;
    P2 = P3;
    P3 = _points[pos];
    next_derivate = (P3 - P2).y;
  }

  // iteration to consider the last element.
  P1 = _points[many_points-1];
  uint64_t y = static_cast<uint64_t>(P1.y);
  uint64_t x = static_cast<uint64_t>(P1.x);
  float q = (_points[0] - _points[many_points-1]).dir_mul(_points[many_points-1] - _points[many_points-2]).y;
  if (q < 0.f)
    B.unset (y, x);
  else if (q > 0.f)
    B.set (y, x);
 
  // creating and printing the image.
  const uint32_t height = std::lround(dims_l.y);
  const uint32_t width = std::lround(dims_l.x);
  SDL_Surface* surface = SDL_CreateRGBSurface (0, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  Uint32* pixels = (Uint32*)(surface->pixels);
  Uint32 pixel_color;

  B.fill_bounds();
  switch (antialias) {
    case AAx1:
      pixel_color = SDL_MapRGBA(surface->format, color.r, color.g, color.b, 255);
      for (uint32_t h = 0; h < height; h++) {
        for (uint32_t w = 0; w < width; w++) {
          if (B(h, w))
            pixels[h * width + w] = pixel_color;
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
          uint64_t elem = B.number_bits(h, w);
          pixels[h * width + w] = pixel_color | ((elem << 2) - (elem > 0));
        }
      }
      break;
    case AAx16: 
      pixel_color = SDL_MapRGBA(surface->format, color.r, color.g, color.b, 0);
      for (uint32_t h = 0, h_2 = 0; h < height; h++, h_2+=2) {
        for (uint32_t w = 0, w_2 = 0; w < width; w++, w_2+=2) {
          uint64_t elem = B.number_bits_quad(h_2, w_2);
          pixels[h * width + w] = pixel_color | (elem - (elem > 0));
        }
      }
      break;
  }

  return surface;
}
