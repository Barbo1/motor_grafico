#include "../../../headers/primitives/glyph_system.hpp"
#include "../../../headers/primitives/bool_matrix.hpp"
#include <SDL2/SDL_stdinc.h>
#include <algorithm>
#include <cstdint>
#include <utility>
#include <iostream>

static float get_directon_for_quad_next (const Dir2& curr, const std::pair<Dir2, uint8_t>& next, const std::pair<Dir2, uint8_t>& need) {
  float ret = (next.first - curr).y;
  if (next.second == 1) {
    return ret;
  } else if (ret == 0.f) {
    if (need.second == 1)
      ret = (need.first - curr).y;
    else
      ret = ((next.first + need.first) * 0.5f - curr).y;
  }
  return ret;
}

static float get_directon_for_quad_prev (const Dir2& curr, const std::pair<Dir2, uint8_t>& next, const std::pair<Dir2, uint8_t>& need) {
  float ret = (curr - next.first).y;
  if (next.second == 1) {
    return ret;
  } else if (ret == 0.f) {
    if (need.second == 1)
      ret = (curr - need.first).y;
    else
      ret = (curr - (next.first + need.first) * 0.5f).y;
  }
  return ret;
}

static inline bool float_is_zero (float a) {
  return -0.0001f < a && a < 0.0001f;
}

static void draw_line (BoolMatrixU& bound, Dir2 P1, Dir2 P2, float& prev_direction, float& next_direction) {
  Dir2 diff21 = P2 - P1;
  int64_t yidiff = std::lround(P2.y) - std::lround(P1.y);
  uint64_t prev_point_disapears = (diff21.y * prev_direction < 0.f ? 1ULL : 0ULL);
  uint64_t next_point_disapears = (diff21.y * next_direction < 0.f ? 1ULL : 0ULL);

  std::cout << "nueva linea <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
  std::cout << "P1: (" << P1.x << ", " << P1.y << ")" << std::endl;
  std::cout << "P2: (" << P2.x << ", " << P2.y << ")" << std::endl;
  std::cout << "---------------------------------" << std::endl;
  std::cout << "diff21: (" << diff21.x << ", " << diff21.y << ")" << std::endl;
  std::cout << "prev_direction: " << prev_direction << std::endl;
  std::cout << "next_direction: " << next_direction << std::endl;
  if (std::abs(yidiff) == 0) {
    if (std::lround (P1.x) != std::lround (P2.x) && 
      !float_is_zero (prev_direction) && 
      !float_is_zero (next_direction) && 
      prev_direction * next_direction > 0.f
    )
      bound.unset (std::lround (P1.y), std::lround (P1.x));
  } else if (std::abs(yidiff) == 1) {
    bound.change (
      std::lround (P1.y), 
      std::lround (P1.x), 
      (prev_point_disapears & bound(std::lround (P1.y), std::lround (P1.x))) ^ 1ULL
    );
    bound.change (
      std::lround (P2.y), 
      std::lround (P2.x), 
      (next_point_disapears & bound(std::lround (P2.y), std::lround (P2.x))) ^ 1ULL
    );
  } else {
    float x_diff = diff21.x / diff21.y;
    if (P1.y > P2.y) {
      std::swap (P1, P2);
      std::swap (prev_point_disapears, next_point_disapears);
    }

    uint64_t mid = (std::lround(P2.y) + std::lround(P1.y)) >> 1;
    float x = P1.x;
    for (uint64_t yi = std::lround (P1.y); yi <= mid; yi++) {
      uint64_t xi = std::lround (x);
      bound.change (yi, xi, (prev_point_disapears & bound(yi, xi)) ^ 1ULL);
      x += x_diff;
    }
      
    x = P2.x;
    for (uint64_t yi = std::lround (P2.y); yi > mid; yi--) {
      uint64_t xi = std::lround (x);
      bound.change (yi, xi, (next_point_disapears & bound(yi, xi)) ^ 1ULL);
      x -= x_diff;
    }
  }
}

enum BezierConfig {
  PrintDIR = 0b1,       // 1 for up
  TRule = 0b10,         // 1 for max
};

static void print_bezier_part (
  BoolMatrixU& bound, 
  const uint32_t bezier_config,
  const Dir2& av,
  const Dir2& bv,
  const float& rem_sqrt,
  const float& iavy,
  const Dir2& P1,
  const Dir2& first,
  int64_t last_height,
  float prev_direction,
  float next_direction,
  float curr_direction
) {
  float t = (bezier_config & BezierConfig::TRule ? 1.f : 0.f);
  bool print = true;
  uint64_t xi = std::lround(first.x);
  const int64_t first_height = std::lround(first.y);
  const int64_t middle_height = (first_height + last_height) >> 1;
    
  std::cout << "prev_dis_cond: " << prev_direction << std::endl;
  std::cout << "next_dis_cond: " << next_direction << std::endl;
  std::cout << "curr_dis_cond: " << curr_direction << std::endl;
  
  uint64_t prev_disappearance_cond = (prev_direction * curr_direction) < 0.f ? 1ULL : 0ULL;
  uint64_t next_disappearance_cond = (next_direction * curr_direction) < 0.f ? 1ULL : 0ULL;

  auto print_points = [&] (uint64_t yi, uint64_t yin, uint64_t disappearance_cond) {
    std::cout << "---------------------------------" << std::endl;
    std::cout << "(xi, yi): (" << xi << ", " << yi << ")" << std::endl;
    std::cout << "yin: " << yin << std::endl;
    std::cout << "disappearance_cond: " << disappearance_cond << std::endl;
    std::cout << "posicion: " << bound(yi, xi) << std::endl;
    std::cout << "need: " << ((disappearance_cond & bound(yi, xi)) ^ 1ULL)<< std::endl;
    std::cout << "t: " << t << std::endl;
    std::cout << "print: " << print << std::endl;
    if (print)
      bound.change (yi, xi, (disappearance_cond & bound(yi, xi)) ^ 1ULL);
    //std::cout << "posicion luego: " << bound(yi, xi) << std::endl;
    
    // test if there is a t for the height yi.
    float sqrt_cond = std::max(std::fmaf (av.y, static_cast<float>(yin), rem_sqrt), 0.f);
    std::cout << "sqrt_cond: " << sqrt_cond << std::endl;
    print = sqrt_cond >= 0.f;
    if (!print) return;
    sqrt_cond = std::sqrt (sqrt_cond);

    // calculate the yi.
    float t1 = iavy * (bv.y - sqrt_cond);
    bool print1 = (0.f < t1) && (t1 < 1.f);

    float t2 = iavy * (bv.y + sqrt_cond);
    bool print2 = (0.f < t2) && (t2 < 1.f);

    print = print1 || print2;
    if (print1 && print2) {
      if (bezier_config & BezierConfig::TRule)
        t = std::max (t1, t2);
      else
        t = std::min (t1, t2);
    } else if (print1)
      t = t1;
    else if (print2)
      t = t2;

    xi = std::lround (std::fmaf (std::fmaf (av.x, t, bv.x), t, P1.x));
  };

  //std::cout << "first height: " << first_height << std::endl;
  //std::cout << "middle height: " << middle_height << std::endl;
  //std::cout << "last height: " << last_height << std::endl;

  if (std::abs(first_height - last_height) == 0) {
    print_points (first_height, 0, prev_disappearance_cond);
  } else if (std::abs(first_height - last_height) == 1) {
    print_points (first_height, last_height, prev_disappearance_cond);
    print_points (last_height, 0, next_disappearance_cond);
  } else {
    int64_t yi = first_height;
    if (bezier_config & BezierConfig::PrintDIR) {
      for (; yi <= middle_height; yi++)
        print_points (yi, yi + 1, prev_disappearance_cond);
      //std::cout << "otra parte. "<<  std::endl;
      for (; yi <= last_height; yi++)
        print_points (yi, yi + 1, next_disappearance_cond);
    } else {
      for (; yi >= middle_height; yi--)
        print_points (yi, yi - 1, prev_disappearance_cond);
      //std::cout << "otra parte. "<<  std::endl;
      for (; yi >= last_height; yi--)
        print_points (yi, yi - 1, next_disappearance_cond);
    }
  }
}
/*
  static void print_bezier_part (
    BoolMatrixU& bound, 
    BezierPrintDir direction, 
    BezierTRule t_rule, 
    const Dir2 av,
    const Dir2 bv,
    uint32_t first,
    uint32_t last,
    uint64_t disappearance_cond
  )
  donde:
    + BezierPrintDir denota la direction hacia donde va la curva(arriba o abajo)
    + BezierTRule denota si se toma el t maximo o minimo(dentro del rango [0, 1]).
    + av y bv se definen como se calculan abajo
    + first y last son las alturas
    + disappearance_cond denota si desaparece o no

  static void draw_quad_bezier (BoolMatrixU& bound, Dir2 P1, Dir2 P2, Dir2 P3, float prev_direction, float next_direction);
*/

static void draw_quad_bezier (BoolMatrixU& bound, Dir2 P1, Dir2 P2, Dir2 P3, float prev_direction, float next_direction) {
  std::cout << "nueva quad <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
  std::cout << "P1 = (" << P1.x << ", " << P1.y << ")"<< std::endl;
  std::cout << "P2 = (" << P2.x << ", " << P2.y << ")"<< std::endl;
  std::cout << "P3 = (" << P3.x << ", " << P3.y << ")"<< std::endl;

  Dir2 av = P3 + P1 - P2 * 2.f;
  Dir2 bv = (P1 - P2).dir_mul(Dir2 {-2.f, 1.f});
  float rem_sqrt = P2.y * P2.y - P1.y * P3.y;
  float iavy = _mm_cvtss_f32 (_mm_permute_ps (_mm_rcp_ps (av.v), 0b01010101));

  float direction_calc_1 = (P2 - P1).y;
  if (float_is_zero (direction_calc_1))
    direction_calc_1 = (P3 - P1).y;

  float direction_calc_3 = (P3 - P2).y;
  if (float_is_zero (direction_calc_3)) 
    direction_calc_3 = (P3 - P1).y;
  
  std::cout << "prev_direction:" << prev_direction << std::endl;
  std::cout << "next_direction:" << next_direction << std::endl;
  std::cout << "direction_calc_1 :" << direction_calc_1 << std::endl;
  std::cout << "direction_calc_3 :" << direction_calc_3 << std::endl;

  uint64_t prev_point_disapears = (direction_calc_1 * prev_direction < 0.f ? 1ULL : 0ULL);
  uint64_t next_point_disapears = (direction_calc_3 * next_direction < 0.f ? 1ULL : 0ULL);

  std::cout << "av.y :" << av.y << std::endl;
  std::cout << "bv.y :" << bv.y << std::endl;

  std::cout << "f1: " << float_is_zero ((P1 - P2).y) << std::endl; 
  std::cout << "f2: " << float_is_zero ((P3 - P1).y) << std::endl; 
  std::cout << "f3: " << ((P1 - P2).dir_mul(P3 - P2).y > 0.f) << std::endl; 

  // cases.
  if (float_is_zero (av.y)) {
    std::cout << "1" << std::endl;
    if (float_is_zero (bv.y)) {
      draw_line (bound, P1, P3, prev_direction, next_direction);
    } else {
      Dir2 nP1 = P1;
      Dir2 nP3 = P3;
      if (P1.y > P3.y) {
        std::swap (prev_point_disapears, next_point_disapears);
        std::swap (nP1, nP3);
      }

      int64_t mid = (std::lround (nP1.y) + std::lround (nP3.y)) >> 1;
      uint64_t xi = std::lround (nP1.x);
      for (int64_t yi = std::lround(nP1.y); yi <= mid; yi++) {
        std::cout << "(xi, yi): (" << xi << ", " << yi << ")" << std::endl;
        std::cout << "prev_point_disapears: (" << prev_point_disapears << std::endl;
        std::cout << "bound(yi, xi): (" << bound(yi, xi) << std::endl;
        bound.change (yi, xi, (prev_point_disapears & bound(yi, xi)) ^ 1ULL);
        float t = 0.5f * (P1.y - static_cast<float>(yi + 1)) / bv.y;
        std::cout << "t: " << t << std::endl;
        xi = std::lround (std::fmaf (std::fmaf (av.x, t, bv.x), t, P1.x));
      }

      xi = std::lround (nP3.x);
      for (int64_t yi = std::lround(nP3.y); yi > mid; yi--) {
        std::cout << "(xi, yi): (" << xi << ", " << yi << ")" << std::endl;
        std::cout << "next_point_disapears: (" << next_point_disapears << std::endl;
        std::cout << "bound(yi, xi): (" << bound(yi, xi) << std::endl;
        bound.change (yi, xi, (next_point_disapears & bound(yi, xi)) ^ 1ULL);
        float t = 0.5f * (P1.y - static_cast<float>(yi - 1)) / bv.y;
        std::cout << "t: " << t << std::endl;
        xi = std::lround (std::fmaf (std::fmaf (av.x, t, bv.x), t, P1.x));
      }
    }

    std::cout << "fin 1" << std::endl;
  } else if (!float_is_zero ((P1 - P2).y) && !float_is_zero ((P3 - P2).y) && (P1 - P2).dir_mul(P3 - P2).y > 0.f) {
    std::cout << "2" << std::endl;
    float t_bound = bv.y * iavy;
    uint64_t yim = std::lround (std::fmaf (std::fmaf (av.y, t_bound, -2.f*bv.y), t_bound, P1.y));
    uint64_t bc1 = 0, bc2 = BezierConfig::TRule;

    if ((P3 - P2).y < 0.f) {
      bc1 |= BezierConfig::PrintDIR;
      bc2 |= BezierConfig::PrintDIR;
    }
    print_bezier_part (
      bound, bc1, av, bv, rem_sqrt, iavy, P1, P1, yim, prev_direction, direction_calc_3, direction_calc_1
    );
    std::cout << "||||||||||||||||||||-->>"<<  std::endl;
    print_bezier_part (
      bound, bc2, av, bv, rem_sqrt, iavy, P1, P3, yim, next_direction, direction_calc_1, direction_calc_3
    );

    std::cout << "fin 2" << std::endl;
    return;
  } else {
    std::cout << "3" << std::endl;

    uint64_t yim = std::lround (((P1 + P3) * 0.5f).y);
    float cond = std::round(P3.y) - std::round(P1.y);
    std::cout << "esto tiene una diferencia de: " << cond << std::endl;

    if (std::abs(cond) == 0.f) {
      if (std::lround (P3.x) != std::lround (P1.x) && prev_direction * next_direction > 0.f)
        bound.unset (std::lround (P1.y), std::lround (P1.x));
    } else if (std::abs(cond) == 1.f) {
      uint64_t rP1y = std::lround (P1.y);
      uint64_t rP1x = std::lround (P1.x);
      uint64_t rP3y = std::lround (P3.y);
      uint64_t rP3x = std::lround (P3.x);
      bound.change (rP1y, rP1x, (prev_point_disapears & bound(rP1y, rP1x)) ^ 1ULL);
      bound.change (rP3y, rP3x, (next_point_disapears & bound(rP3y, rP3x)) ^ 1ULL);
    } else if (0.f < cond) {
      std::cout << "a" << std::endl;
      print_bezier_part (
        bound, BezierConfig::PrintDIR, 
        av, bv, rem_sqrt, iavy, P1, P1, yim, prev_direction, direction_calc_3, direction_calc_1
      );
      std::cout << "||||||||||||||||||||-->>"<<  std::endl;
      print_bezier_part (
        bound, BezierConfig::TRule, 
        av, bv, rem_sqrt, iavy, P1, P3, yim+1, next_direction, direction_calc_1, direction_calc_3
      );
      std::cout << "<<--||||||||||||||||||||"<<  std::endl;
    } else if (cond < 0.f) {
      std::cout << "b" << std::endl;
      print_bezier_part (
        bound, 0, 
        av, bv, rem_sqrt, iavy, P1, P1, yim, prev_direction, direction_calc_3, direction_calc_1
      );
      std::cout << "||||||||||||||||||||-->>"<<  std::endl;
      print_bezier_part (
        bound, BezierConfig::TRule | BezierConfig::PrintDIR, 
        av, bv, rem_sqrt, iavy, P1, P3, yim-1, next_direction, direction_calc_1, direction_calc_3
      );
      std::cout << "<<--||||||||||||||||||||"<<  std::endl;
    }
    std::cout << "fin 3" << std::endl;
  }
}

SDL_Surface* GlyphsSystem::raster (char16_t character, uint32_t s) {
  auto founded = this->mapping.find (character);
  if (founded == this->mapping.end())
    return nullptr;

  uint32_t glyph_index = founded->second;
  float sizef = static_cast<float>(s);
  const ttf_glyph_simple_data& data = std::get<ttf_glyph_simple_data>(this->glyphs[glyph_index].raster_information);

  Dir2 min = this->glyphs[glyph_index].bounding_box.first;
  Dir2 max = this->glyphs[glyph_index].bounding_box.second;
  Dir2 dims_reposition = (max - min) * sizef;
  Dir2 dims = dims_reposition + Dir2 {8.f, 8.f};
  dims = Dir2 (std::round (dims.x), std::round (dims.y));
 
  // dimentions of the bit matrix.
  int32_t matrix_height = std::lround (dims.y);
  int32_t matrix_width = std::lround (dims.x);
  if (matrix_width <= 0 || matrix_height <= 0)
    return nullptr;
    
  std::cout << "dims: (" << dims.x << ", " << dims.y << ")" << std::endl;

  // generating array of the point of the scaled glyph.
  std::vector<std::vector<std::pair<Dir2, uint8_t>>> points (data.offset.size ());
  uint32_t curr_off = 0, pos = 0;
  for (auto& off: data.offset) {
    uint32_t many = off - curr_off;
    points[pos].resize (many + 4);

    // there are not two consecutive straight lines.
    if ((data.flags[curr_off] & 1) && (data.flags[curr_off+1] & 1) && data.points[curr_off].y == data.points[curr_off+1].y) {
      for (uint32_t i = 0; i < many; i++) {
        Dir2 p = (data.points[i + 1 + curr_off] - min) * sizef;
        p.y = (dims_reposition - p).y;
        points[pos][i] = std::pair<Dir2, uint8_t> (p, data.flags[i + 1 + curr_off] & 1);
      }
      Dir2 p = (data.points[curr_off] - min) * sizef;
      p.y = (dims_reposition - p).y;
      points[pos][many] = std::pair<Dir2, uint8_t> (p, data.flags[curr_off] & 1);
    } else {
      for (uint32_t i = 0; i <= many; i++) {
        Dir2 p = (data.points[i + curr_off] - min) * sizef;
        p.y = (dims_reposition - p).y;
        points[pos][i] = std::pair<Dir2, uint8_t> (p, data.flags[i + curr_off] & 1);
      }
    }

    points[pos][many+1] = points[pos][0];
    points[pos][many+2] = points[pos][1];
    points[pos][many+3] = points[pos][2];

    curr_off = off + 1;
    pos++;
  }

  // creating matrix of bits.
  BoolMatrixU B = BoolMatrixU (matrix_height, matrix_width);

  for (const auto& vec: points) {
    std::cout << std::endl << "-------------------------" << std::endl;
    std::cout << "nueva componente" << std::endl;
    std::cout << "-------------------------" << std::endl<< std::endl ;
    uint32_t pos = 0, next_pos = 1;
    uint32_t many = vec.size() - 4;

    Dir2 P1 = (vec[pos].second ? 
      Dir2{} : 
      (vec[many].second ? vec[many].first : (vec[many].first + vec[pos].first) * 0.5f)
    );
    std::pair<Dir2, uint8_t> P2 = vec[pos];
    std::pair<Dir2, uint8_t> P3 = vec[next_pos];
    Dir2 PA = Dir2 {};

    float next_direction, prev_direction;
    if (vec[pos].second)
      prev_direction = get_directon_for_quad_prev(P2.first, vec[many], vec[many-1]);
    else if (vec[many].second)
      prev_direction = get_directon_for_quad_prev(P1, vec[many-1], vec[many-2]);
    else
      prev_direction = get_directon_for_quad_prev(P1, vec[many], vec[many-1]);

    while (pos <= many) {
      switch ((vec[pos].second << 1) | vec[next_pos].second) {
        case 0b00:
          PA = (P3.first + P2.first) * 0.5f;
          next_direction = get_directon_for_quad_next (PA, P3, vec[next_pos+1]);
          draw_quad_bezier (B, P1, P2.first, PA, prev_direction, next_direction);
          prev_direction = get_directon_for_quad_prev(PA, P2, std::pair<Dir2, uint8_t>({P1, 1}));
          P1 = PA;
          break;
        case 0b01:
          next_direction = get_directon_for_quad_next (P3.first, vec[next_pos+1], vec[next_pos+2]);
          draw_quad_bezier (B, P1, P2.first, P3.first, prev_direction, next_direction);
          prev_direction = get_directon_for_quad_prev(P3.first, P2, std::pair<Dir2, uint8_t>({P1, 1}));
          break;
        case 0b11:
          next_direction = get_directon_for_quad_next (P3.first, vec[next_pos+1], vec[next_pos+2]);
          draw_line (B, P2.first, P3.first, prev_direction, next_direction);
          prev_direction = (P3.first - P2.first).y;
        case 0b10:
          P1 = P2.first;
          break;
      }
      pos++; next_pos++;
      P2 = P3;
      P3 = vec[next_pos];
    }
  }

  Dir2 dims_l = (max - min).madd(s, Dir2{1.f, 1.f});
  uint32_t height = std::lround (dims_l.y);
  uint32_t width = std::lround (dims_l.x);
  std::cout << "image height: " << height << std::endl;
  std::cout << "image width: " << width << std::endl;
  SDL_Surface* surface = SDL_CreateRGBSurface (0, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  std::cout << "*_*__*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_**_*_*_**_esto 1." << std::endl;
  Uint32* pixels = (Uint32*)surface->pixels;

  B.fill_bounds();
  for (uint32_t h = 0; h < height; h++) {
    for (uint32_t w = 0; w < width; w++) {
      uint64_t elem = B(h, w);
      *(pixels++) = 0xFFFFFF00 | static_cast<uint32_t>((elem << 8) - elem);
    }
  }

  return surface;
}
