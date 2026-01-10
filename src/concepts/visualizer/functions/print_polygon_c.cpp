#include "../../../../headers/concepts/visualizer.hpp"
#include "../../../../headers/primitives/bool_matrix.hpp"
#include <cmath>
#include <cstdint>

static void draw_line (BoolMatrixS& bound, Dir2& P1, Dir2& P2, float& prev_derivate) {
  uint32_t bottom = std::lround (_mm_cvtss_f32 (_mm_permute_ps (_mm_min_ps (P1.v, P2.v), 0b01010101)));
  uint32_t top = std::lround (_mm_cvtss_f32 ( _mm_permute_ps (_mm_max_ps (P1.v, P2.v), 0b01010101)));
  Dir2 diff21 = P2 - P1;

  if (-0.0001f < diff21.x && diff21.x < 0.0001f) {
    for (uint32_t y = bottom; y <= top; y++)
      bound.set (y, std::lround (P1.x));
  } else if (-0.0001f > diff21.y || diff21.y > 0.0001f) {
    float m = 1.f / diff21.y;
    float r = -P1.y * m;

    for (uint32_t y = bottom; y <= top; y++) {
      float t = std::fmaf (static_cast<float>(y), m, r);
      if (0.0001f < t && t < 0.9999f) {
        uint32_t x = std::lround (
          std::fmaf (diff21.x, t, P1.x)
        );
        if (diff21.y * prev_derivate < 0.0001f && bound(y, x))
          bound.unset (y, x);
        else
          bound.set (y, x);
      }
    }
  }

  prev_derivate = diff21.y;
}

void print_polygon_c (Global* glb, std::vector<Dir2> points, SDL_Color color) {
  std::size_t many_points = points.size();
    
  // Searching maximum and minimum coordenates.
  Dir2 min = points[0], max = points[0];
  for (const auto& point: points) {
    min.v = _mm_min_ps (min.v, point.v);
    max.v = _mm_max_ps (max.v, point.v);
  }

  Dir2 min_sized = min * 16.f;
  Dir2 dims_l = (max - min);
  Dir2 dims = dims_l.madd(16.f, Dir2{16.f, 16.f});

  int32_t matrix_height = std::lround (dims.y);
  int32_t matrix_width = std::lround (dims.x);
  BoolMatrixS B = BoolMatrixS (matrix_height, matrix_width);

  // generating array of the point of the scaled glyph.
  std::vector<Dir2> _points (many_points+1);
  for (std::size_t i = 0; i < many_points; i++) {
    _points[i] = points[i].msub(16.f, min_sized);
  }
  _points[many_points] = points[0].msub(16.f, min_sized);

  float derivate = (_points[many_points-1] - _points[many_points-2]).y;
  Dir2 P1 = _points[many_points-1];
  Dir2 P2 = _points[0];
  for (std::size_t pos = 0; pos < many_points; pos++) {
    draw_line (B, P1, P2, derivate);
    P1 = _points[pos];
    P2 = _points[pos+1];
  }
  
  const uint32_t height = std::lround(dims_l.y);
  const uint32_t width = std::lround(dims_l.x);
  Uint32* pixels = new Uint32[height * width];
  std::memset (pixels, 0, height * width * sizeof (Uint32));

  B.fill_bounds();
  SDL_SetRenderDrawBlendMode(glb->get_render(), SDL_BLENDMODE_ADD);

  const uint32_t w_offset = static_cast<uint32_t> (min.x);
  const uint32_t h_offset = static_cast<uint32_t> (min.y);

  for (uint32_t h = 0, h_2 = 0; h < height; h++, h_2+=2) {
    for (uint32_t w = 0, w_2 = 0; w < width; w++, w_2+=2) {
      long num = B.number_bits_quad(h_2, w_2);
      SDL_SetRenderDrawColor(glb->get_render(), color.r, color.g, color.b, std::min (num, (long)255));
      SDL_RenderDrawPoint (glb->get_render(), w + w_offset, h + h_offset);
    }
  }
}
