#include "../../../headers/primitives/glyph_system.hpp"
#include "../../../headers/primitives/bool_matrix.hpp"
#include <algorithm>

static void draw_line (BoolMatrixS& bound, Dir2& P1, Dir2& P2, float& prev_derivate) {
  float bottom = _mm_cvtss_f32 (_mm_permute_ps (_mm_min_ps (P1.v, P2.v), 0b01010101));
  float top = _mm_cvtss_f32 ( _mm_permute_ps (_mm_max_ps (P1.v, P2.v), 0b01010101));
  Dir2 diff21 = P2 - P1;

  if (-0.0001f > diff21.y || diff21.y > 0.0001f) {
    float m = 1.f / diff21.y;
    float r = -P1.y * m;

    for (float y_d = bottom; y_d <= top; y_d += 1.f) {
      float t = std::fmaf (y_d, m, r);

      if (-0.0001f < t && t < 1.0001f) {
        uint32_t y = std::min(std::lround (y_d), (long)bound.get_height() - 1);
        uint32_t x = std::min (
          std::lround (std::fmaf(diff21.x, t, P1.x)), 
          (long)bound.get_width() - 1
        );
        if (diff21.x * diff21.y * prev_derivate < -0.0001f && bound(y, x))
          bound.unset (y, x);
        else
          bound.set (y, x);
      }
    }
    prev_derivate = diff21.y * diff21.x;
  } else {
    uint32_t y = std::min(std::lround (P1.y), (long)bound.get_height() - 1);
    bound.set (y, std::min(std::lround (P1.x), (long)bound.get_width() - 1));
    bound.set (y, std::min(std::lround (P2.x), (long)bound.get_width() - 1));
    prev_derivate = 0.f;
  }
}

static void draw_quad_bezier (BoolMatrixS& bound, Dir2& P1, Dir2& P2, Dir2& P3, float& prev_derivate) {
  Dir2 av = P3 + P1 - P2 * 2.f;
  Dir2 bv = (P3 - P2).dir_mul(Dir2 {-2.f, 1.f});
  float rem_sqrt = P2.y * P2.y - P1.y * P3.y;
  float iavy = _mm_cvtss_f32 ( _mm_permute_ps (_mm_rcp_ps (av.v), 0b01010101));

  float dir = bv.y < 0.0001f ? 1.f : -1.f;
  bool quad = -0.0001f < av.y && av.y < 0.0001f;

  float t_bound = bv.y * iavy;
  uint32_t s_m, s_M, f_m, f_M;

  /*
  std::cout << "av: (" << av.x << "," << av.y << ")" << std::endl;
  std::cout << "bv: (" << (P3 - P2).x << "," << (P3 - P2).y << ")" << std::endl;
  std::cout << "bv cambio: (" << bv.x << "," << bv.y << ")" << std::endl;
  std::cout << "P1: (" << P1.x << ", " << P1.y << ")" << std::endl;
  std::cout << "P2: (" << P2.x << ", " << P2.y << ")" << std::endl;
  std::cout << "P3: (" << P3.x << ", " << P3.y << ")" << std::endl;
  std::cout << "t_bound: " << t_bound << std::endl;
  */
  if (0.0001f < t_bound && t_bound < 0.9999f && !quad) {
    uint32_t yi = std::lround (std::fmaf (std::fmaf (av.y, t_bound, -2.f*bv.y), t_bound, P3.y));
    uint32_t y1 = std::lround (P1.y);
    uint32_t y3 = std::lround (P3.y);
    
    s_m = std::lround (std::min (yi, y3));
    f_m = std::lround (std::max (yi, y3));
    s_M = std::lround (std::min (yi, y1));
    f_M = std::lround (std::max (yi, y1));
    //std::cout << 1 << std::endl;
  } else {
    s_m = std::lround (_mm_cvtss_f32 (_mm_permute_ps (_mm_min_ps (P1.v, P3.v), 0b01010101)));
    f_m = std::lround (_mm_cvtss_f32 (_mm_permute_ps (_mm_max_ps (P1.v, P3.v), 0b01010101)));
    s_M = 1;
    f_M = 0;
    //std::cout << 2 << std::endl;
  }
  /*
  std::cout << "s_m: " << s_m << std::endl;
  std::cout << "f_m: " << f_m << std::endl;
  std::cout << "s_M: " << s_M << std::endl;
  std::cout << "f_M: " << f_M << std::endl;
  */

  for (uint32_t yi = s_m; yi <= f_m; yi++) {
    float t;
    if (quad)
      t = 0.5f * (P3.y - static_cast<float>(yi)) / bv.y;
    else {
      t = std::fmaf (av.y, static_cast<float>(yi), rem_sqrt);
      //std::cout << "--: " << t << std::endl;
      if (t < 0.f) continue;
      t = iavy * std::fmaf(dir, std::sqrt (t), bv.y);
    }
    if (-0.0001f < bv.y && bv.y < 0.0001f)
      t = std::abs (t);
    //std::cout << "t obtenido: " << t << std::endl;

    if (-0.0001f < t && t < 1.0001f) {
      float y = std::min ((long)yi, (long)bound.get_height() - 1);
      float x = std::min (
        std::lround (std::fmaf (std::fmaf (av.x, t, bv.x), t, P3.x)), 
        (long)bound.get_width () - 1
      );
      if (std::fmaf (-av.y, t, bv.y) * prev_derivate < -0.0001f && bound(y, x))
        bound.unset (y, x);
      else
        bound.set (y, x);
    }
  }

  //std::cout << "." << std::endl;
  for (uint32_t yi = s_M; yi <= f_M; yi++) {
    float t = std::fmaf (av.y, static_cast<float>(yi), rem_sqrt);
    //std::cout << "--: " << t << std::endl;
    if (t < 0.f) continue;
    t = iavy * std::fmaf(-dir, std::sqrt (t), bv.y);

    if (-0.0001f < bv.y && bv.y < 0.0001f)
      t = std::abs (t);
    //std::cout << "t obtenido: " << t << std::endl;

    if (-0.0001f < t && t < 1.0001f) {
      float y = std::min ((long)yi, (long)bound.get_height() - 1);
      float x = std::min (
        std::lround (std::fmaf (std::fmaf (av.x, t, bv.x), t, P3.x)), 
        (long)bound.get_width () - 1
      );
      if (std::fmaf (-av.y, t, bv.y) * prev_derivate < -0.0001f && bound(y, x))
        bound.unset (y, x);
      else
        bound.set (y, x);
    }
  }

  Dir2 this_derivate = P3 - P2;
  this_derivate = (
    -0.0001f < this_derivate.y && this_derivate.y < 0.0001f && 
    -0.0001f < this_derivate.x && this_derivate.x < 0.0001f ? 
    P3 - P1 : 
    this_derivate
  );
  prev_derivate = this_derivate.y * this_derivate.x;
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
  int32_t matrix_width = std::lround (dims.x);
  if (matrix_width <= 0 || matrix_height <= 0) 
    return nullptr;
  BoolMatrixS B = BoolMatrixS (matrix_height, matrix_width);

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

    while (pos < many) {
      switch ((data.flags[pos] & 0b1) << 1 | (data.flags[next_pos] & 0b1)) {
        case 0b00:
          PA = (P3 + P2) * 0.5f;
          draw_quad_bezier (B, P1, P2, PA, derivate);
          P1 = PA;
          break;
        case 0b01:
          draw_quad_bezier (B, P1, P2, P3, derivate);
          break;
        case 0b11:
          draw_line (B, P2, P3, derivate);
        case 0b10:
          derivate = 0.f;
          P1 = P2;
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
        draw_quad_bezier (B, P1, P2, P3, derivate);
        break;
      case 0b11:
        if (P2.y != P3.y)
          draw_line (B, P2, P3, derivate);
        else
          derivate = 0.f;
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
  /*
  uint32_t height = B.get_height();
  uint32_t width = B.get_width();
  Uint32* pixels = new Uint32[height * width];
  std::memset (pixels, 0, height * width * sizeof (Uint32));

  B.fill_bounds();

  for (uint32_t h = 0, h_2 = 0; h < height; h++, h_2+=2) {
    for (uint32_t w = 0, w_2 = 0; w < width; w++, w_2+=2) {
      if (B(h, w))
        pixels[h * width + w] = 0xFFFFFFFF;
    }
  }
  */
  
  SDL_Surface* surface = SDL_CreateRGBSurfaceFrom (
    pixels, (Uint32)width, (Uint32)height, 32, (Uint32)width * 4, 
    0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
  );

  return surface;
}
