#include "../../../../headers/primitives/glyph_system.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>

static uint32_t draw_line (TTFBoolMatrix& bound, Dir2& P1, Dir2& P2, uint32_t last_height, float& prev_derivate) {
  float bottom = _mm_cvtss_f32 (_mm_permute_ps (_mm_min_ps (P1.v, P2.v), 0b01010101));
  float top = _mm_cvtss_f32 ( _mm_permute_ps (_mm_max_ps (P1.v, P2.v), 0b01010101));
  Dir2 diff21 = P2 - P1;

  uint32_t ret_y = -1;
  if (diff21.y != 0) {
    float m = 1.f / diff21.y;
    float r = -P1.y * m;

    float y_d;
    uint32_t greater_t = 0;
    for (y_d = bottom; y_d < top; y_d += 1.f) {
      float t = std::fmaf (y_d, m, r);
      uint32_t yi = std::min(std::lround (y_d), (long)bound.get_height() - 1);
      uint32_t xi = std::min (
        std::lround (std::fmaf(diff21.x, t, P1.x)), 
        (long)bound.get_width() - 1
      );

      if (-0.001f < t && t < 1.001f) {
        if (t > greater_t) { greater_t = t; ret_y = yi; }
        if (yi != last_height) { bound.set (yi, xi); }
      }
    }
  }

  prev_derivate = (P2 - P1).y;
  return ret_y;
}

static uint32_t draw_quad_bezier (TTFBoolMatrix& bound, Dir2& P1, Dir2& P2, Dir2& P3, uint32_t last_height, float& prev_derivate) {
  Dir2 av = P3 + P1 - P2 * 2.f;
  Dir2 bv = (P3 - P2).dir_mul(Dir2 {-2.f, 1.f});
  float rem_sqrt = P2.y * P2.y - P1.y * P3.y;
  float iavy = _mm_cvtss_f32 ( _mm_permute_ps (_mm_rcp_ps (av.v), 0b01010101));

  float dir = bv.y < 0.0001f ? 1.f : -1.f;
  bool quad = -0.0001f < av.y && av.y < 0.0001f;

  float t_bound = bv.y * iavy;
  uint32_t s_m, s_M, f_m, f_M;
  if (0.0001f < t_bound && t_bound < 0.9999f && !quad) {
    uint32_t yi = std::lround (std::fmaf (std::fmaf (av.y, t_bound, -2.f*bv.y), t_bound, P3.y));
    uint32_t y1 = std::lround (P1.y);
    uint32_t y3 = std::lround (P3.y);
    
    s_m = std::lround (std::min (yi, y3));
    f_m = std::lround (std::max (yi, y3));
    s_M = std::lround (std::min (yi, y1));
    f_M = std::lround (std::max (yi, y1));
  } else {
    s_m = std::lround (_mm_cvtss_f32 (_mm_permute_ps (_mm_min_ps (P1.v, P3.v), 0b01010101)));
    f_m = std::lround (_mm_cvtss_f32 (_mm_permute_ps (_mm_max_ps (P1.v, P3.v), 0b01010101)));
    s_M = 1;
    f_M = 0;
  }

  for (uint32_t yi = s_m; yi <= f_m; yi++) {
    float t;
    if (quad)
      t = 0.5f * (P3.y - static_cast<float>(yi)) / bv.y;
    else {
      t = std::fmaf (av.y, static_cast<float>(yi), rem_sqrt);
      if (t < 0.f) continue;
      t = iavy * std::fmaf(dir, std::sqrt (t), bv.y);
    }
    if (-0.0001f < bv.y && bv.y < 0.0001f)
      t = std::abs (t);

    if (-0.0001f < t && t < 1.0001f) {
      float y = std::min ((long)yi, (long)bound.get_height() - 1);
      float x = std::min (
        std::lround (std::fmaf (std::fmaf (av.x, t, bv.x), t, P3.x)), 
        (long)bound.get_width () - 1
      );
      bound.set (y, x);
    }
  }

  for (uint32_t yi = s_M; yi <= f_M; yi++) {
    float t = std::fmaf (av.y, static_cast<float>(yi), rem_sqrt);
    if (t < 0.f) continue;
    t = iavy * std::fmaf(dir, std::sqrt (t), bv.y);

    if (-0.0001f < bv.y && bv.y < 0.0001f)
      t = std::abs (t);

    if (-0.0001f < t && t < 1.0001f) {
      float y = std::min ((long)yi, (long)bound.get_height() - 1);
      float x = std::min (
        std::lround (std::fmaf (std::fmaf (av.x, t, bv.x), t, P3.x)), 
        (long)bound.get_width () - 1
      );
      bound.set (y, x);
    }
  }
  
  uint32_t yi = std::min(std::lround (P1.y), (long)bound.get_height() - 1);
  float this_derivate = (P2 - P1).y;
  if (this_derivate == 0.f)
    this_derivate = (P3 - P1).y;
  if (prev_derivate * this_derivate < 0.f)
    bound.unset(yi, std::min(std::lround (P1.x), (long)bound.get_width() - 1));

  prev_derivate = this_derivate;

  return std::lround(f_M);
}

SDL_Surface* GlyphsSystem::raster (char16_t character, uint32_t s) {
  auto founded = this->mapping.find (character);
  if (founded == this->mapping.end())
    return nullptr;

  uint32_t glyph_index = founded->second;
  float sizef = static_cast<float>(s * 16);
  ttf_glyph_simple_data& data = std::get<ttf_glyph_simple_data>(this->glyphs[glyph_index].raster_information);

  Dir2 min = this->glyphs[glyph_index].bounding_box.first;
  Dir2 max = this->glyphs[glyph_index].bounding_box.second;
  Dir2 dims = (max - min).madd(sizef, Dir2 {16.f, 16.f});

  // Matrix of bits that stores a reduced image of the size 8 times the 
  // image, so that an antialiasing can be applied after.
  // 
  //      ---------------------
  //      |                   |
  //   p1 *                   * p2
  //      |                   |
  //      |                   |
  //      |                   |
  //      |                   |
  //      ---------------------
  int32_t matrix_height = std::lround (dims.y);
  int32_t matrix_width = std::lround (dims.y);
  if (matrix_width <= 0 || matrix_height <= 0) 
    return nullptr;
  TTFBoolMatrix B = TTFBoolMatrix (matrix_height, matrix_width);

  // generating array of the point of the scaled glyph.
  int32_t many_points = data.points.size();
  std::vector<Dir2> points (many_points + 1);
  for (int32_t i = 0; i < many_points; i++) {
    points[i] = data.points[i].msub(sizef, min * sizef);
    points[i].y = (dims - points[i]).y;
    points[i] = points[i].max0();
  }

  float derivate = 0.f;
  int32_t pos = 0, first_pos, next_pos;
  for (auto& many: data.offset) {
    first_pos = pos;
    next_pos = pos + 1;

    Dir2 P1 = (data.flags[pos] & 0b1 ? 
      Dir2{} : 
      (data.flags[many] & 0b1 ? points[many] : (points[many] + points[pos]) * 0.5f)
    );
    Dir2 P2 = points[pos];
    Dir2 P3 = points[next_pos];
    Dir2 PA = Dir2 {};

    uint32_t last_height = -1;

    while (pos < many) {
      switch ((data.flags[pos] & 0b1) << 1 | (data.flags[next_pos] & 0b1)) {
        case 0b00:
          PA = (P3 + P2) * 0.5f;
          last_height = draw_quad_bezier (B, P1, P2, PA, last_height, derivate);
          P1 = PA;
          break;
        case 0b01:
          last_height = draw_quad_bezier (B, P1, P2, P3, last_height, derivate);
          break;
        case 0b11:
          last_height = draw_line (B, P2, P3, last_height, derivate);
        case 0b10:
          P1 = P2;
          last_height = -1;
          break;
      }
      pos++; next_pos++;
      P2 = P3;
      P3 = points[next_pos];
    }

    P3 = points[first_pos];

    switch ((data.flags[many] & 0b1) << 1 | (data.flags[first_pos] & 0b1)) {
      case 0b00:
        P3 = (P3 + P2) * 0.5f;
      case 0b01:
        last_height = draw_quad_bezier (B, P1, P2, P3, last_height, derivate);
        break;
      case 0b11:
        if (P2.y != P3.y)
          last_height = draw_line (B, P2, P3, last_height, derivate);
        break;
    }
    pos++;
  }

  Dir2 dims_l = (max - min) * s + Dir2{1.f, 1.f};
  uint32_t height = std::lround(dims_l.y);
  uint32_t width = std::lround(dims_l.x);
  Uint32* pixels = new Uint32[height * width];
  std::memset (pixels, 0, height * width * sizeof (Uint32));

  B.fill_bounds();

  for (uint32_t h = 0, h_2 = 0; h < height; h++, h_2+=2) {
    for (uint32_t w = 0, w_2 = 0; w < width; w++, w_2+=2) {
      long num = 
        B.number_bits(h_2, w_2) + B.number_bits(h_2+1, w_2) + 
        B.number_bits(h_2, w_2+1) + B.number_bits(h_2+1, w_2+1);
      pixels[h * width + w] = 0xFFFFFF00 | std::min (num, (long)255);
    }
  }
  
  SDL_Surface* surface = SDL_CreateRGBSurfaceFrom (
    pixels, (Uint32)width, (Uint32)height, 32, (Uint32)width * 4, 
    0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
  );

  return surface;
}
