#include "../../../../headers/primitives/glyph_system.hpp"

static uint32_t draw_line (TTFBoolMatrix& bound, Dir2& P1, Dir2& P2, uint32_t last_height, float& prev_derivate) {
  float bottom = _mm_cvtss_f32 (_mm_permute_ps (_mm_min_ps (P1.v, P2.v), 0b01010101));
  float top = _mm_cvtss_f32 ( _mm_permute_ps (_mm_max_ps (P1.v, P2.v), 0b01010101));
  Dir2 diff21 = P2 - P1;

  uint32_t ret_y = -1;
  if (diff21.y != 0) {
    float m = 1.f / diff21.y;
    float r = -P1.y * m;

    float y_d;
    uint32_t greater_t = 0, yi;
    for (y_d = bottom, yi = std::roundl (y_d); y_d < top; y_d += 1.f, yi++) {
      float t = std::fmaf (y_d, m, r);
      if (-0.001f < t && t < 1.001f) {
        if (t > greater_t) { greater_t = t; ret_y = yi; }
        if (yi != last_height) { bound.set (yi, std::roundl (std::fmaf(diff21.x, t, P1.x))); }
      }
    }
  } else {
    ret_y = std::roundl (P1.y);
    bound.set (ret_y, std::roundl (P1.x));
    bound.set (ret_y, std::roundl (P2.x));
  }

  prev_derivate = diff21.y * diff21.x;

  return ret_y;
}

static uint32_t draw_quad_bezier (TTFBoolMatrix& bound, Dir2& P1, Dir2& P2, Dir2& P3, uint32_t last_height, float& prev_derivate) {
  Dir2 av = P1 - P2 * 2.f + P3;
  Dir2 bv = (P1 - P2).dir_mul(Dir2 {-2.f, 1.f});
  float rem_sqrt = P2.y * P2.y - P1.y * P3.y;
  float iavy = _mm_cvtss_f32 ( _mm_permute_ps (_mm_rcp_ps (av.v), 0b01010101));

  float top = _mm_cvtss_f32 ( _mm_permute_ps (_mm_max_ps (_mm_max_ps (P1.v, P2.v), P3.v), 0b01010101));
  float y_d = _mm_cvtss_f32 (_mm_permute_ps (_mm_min_ps (_mm_min_ps (P1.v, P2.v), P3.v), 0b01010101));
  
  // seguramente me puedo deshacer de esto.
  float prev_t;
  while (y_d < top && (prev_t = std::fmaf (av.y, y_d, rem_sqrt)) < 0.f)
    y_d += 1.f;
  // -------------------------------------

  float greater_t = 0.f;
  uint32_t ret_y = 0;
  while (y_d < top && (prev_t = std::fmaf (av.y, y_d, rem_sqrt)) >= 0.f) {
    float sqrt_prev_t = std::sqrt (prev_t);
    uint32_t yi = std::roundl (y_d);

    float t1 = iavy * (bv.y + sqrt_prev_t);
    float t2 = iavy * (bv.y - sqrt_prev_t);

    uint32_t xi1 = std::roundl (std::fmaf (std::fmaf (av.x, t1, bv.x), t1, P1.x));
    uint32_t xi2 = std::roundl (std::fmaf (std::fmaf (av.x, t2, bv.x), t2, P1.x));

    if (xi1 != xi2) {
      if (-0.0001f < t1 && t1 < 1.0001f) {
        if (t1 > greater_t) { greater_t = t1; ret_y = yi; }
        if (yi != last_height || t1 > 0.0001f) bound.set (yi, xi1);
      }
      if (-0.0001f < t2 && t2 < 1.0001f) {
        if (t2 > greater_t) { greater_t = t2; ret_y = yi; }
        if (yi != last_height || t2 > 0.0001f) bound.set (yi, xi2);
      }
    }

    y_d += 1.f;
  }

  uint32_t yi = std::roundl (P1.y);
  float this_derivate = (P2 - P1).y * (P2 - P1).x;
  if (yi == last_height && prev_derivate * this_derivate < 0.f)
    bound.unset(yi, std::roundl (P1.x));
  prev_derivate = this_derivate;

  return ret_y;
}

SDL_Surface* GlyphsSystem::raster (char16_t character, uint32_t s) {
  uint32_t glyph_index = this->mapping[character];
  float sizef = static_cast<float>(s);
  ttf_glyph_simple_data& data = std::get<ttf_glyph_simple_data>(this->glyphs[glyph_index].raster_information);

  Dir2 min = this->glyphs[glyph_index].bounding_box.first;
  Dir2 max = this->glyphs[glyph_index].bounding_box.second;
  Dir2 img_dims = (max - min) * sizef;
  Dir2 dims = img_dims.madd(8.f, Dir2 {1.f, 1.f});

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
  TTFBoolMatrix B = TTFBoolMatrix (static_cast<uint32_t>(dims.y), static_cast<uint32_t>(dims.x));

  // generating array of the point of the scaled glyph.
  int32_t many_points = data.points.size();
  std::vector<Dir2> points (many_points + 1);
  for (int32_t i = 0; i < many_points; i++) {
    points[i] = data.points[i].msub(sizef * 8.f, min * sizef * 8.f);
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

  SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat (
    0, 
    static_cast<uint32_t>(img_dims.x) + 1, 
    static_cast<uint32_t>(img_dims.y) + 1, 
    32, 
    SDL_PIXELFORMAT_RGBA8888
  );
  Uint32* pixels = (Uint32*)surface->pixels;

  B.fill_bounds ();
  for (uint32_t h = 0; h < static_cast<uint32_t>(surface->h); h++) {
    for (uint32_t w = 0; w < static_cast<uint32_t>(surface->w); w++) {
      uint64_t elem = B.number_bits(h, w);
      if (elem > 16) {
        uint32_t alpha = static_cast<uint8_t>(255.f * static_cast<float>(elem) / 64.f);
        pixels[h * surface->w + w] = 0xFFFFFF00 | alpha;
      }
    }
  }

  return surface;
}
