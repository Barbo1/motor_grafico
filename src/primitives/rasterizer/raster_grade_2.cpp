#include "../../../headers/primitives/rasterizer.hpp"
#include <SDL2/SDL_stdinc.h>
#include <algorithm>
#include <cstdint>
#include <utility>

static inline bool is_horizontal_line (const ComponentElement& elem) {
  return elem.t == ComponentElementType::LINE && (elem.start.round() - elem.end.round()).y == 0.f;
}

static inline float get_direction_line (const Dir2& curr, const Dir2& next) {
  return (curr - next).y;
};

static inline float get_direction_quad (const Dir2& curr, const Dir2& next, const Dir2& need) {
  float ret = (curr - next).y;
  if (ret == 0.f)
    ret = (curr - need).y;
  return ret;
}

static inline float get_prev_direction (ComponentElement elem) {
  switch (elem.t) {
    case ComponentElementType::CURVE:
      return get_direction_quad (elem.end, elem.middle, elem.start);
    case ComponentElementType::LINE:
      return get_direction_line (elem.end, elem.start);
  }
  return 0.f;
}

static inline float get_next_direction (ComponentElement elem) {
  switch (elem.t) {
    case ComponentElementType::CURVE:
      return -get_direction_quad (elem.start, elem.middle, elem.end);
    case ComponentElementType::LINE:
      return get_direction_line (elem.end, elem.start);
  }
  return 0.f;
}

static inline bool float_is_zero (float a) {
  return -0.0001f < a && a < 0.0001f;
}

static void draw_line (BoolMatrixU& bound, Dir2 P1, Dir2 P2, const float& prev_direction, const float& next_direction) {
  Dir2 P1r = P1.round();
  Dir2 P2r = P2.round();
  Dir2 diff21 = P2 - P1;
  int64_t P2ry = static_cast<int64_t>(P2r.y);
  int64_t P1ry = static_cast<int64_t>(P1r.y);
  int64_t P2rx = static_cast<int64_t>(P2r.x);
  int64_t P1rx = static_cast<int64_t>(P1r.x);
  int64_t yidiff = P2ry - P1ry;
  uint64_t prev_point_disapears = (diff21.y * prev_direction < 0.f ? 1ULL : 0ULL);
  uint64_t next_point_disapears = (diff21.y * next_direction < 0.f ? 1ULL : 0ULL);

  /*
  std::cout << "nueva linea <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
  std::cout << "P1: (" << P1.x << ", " << P1.y << ")" << std::endl;
  std::cout << "P2: (" << P2.x << ", " << P2.y << ")" << std::endl;
  std::cout << "---------------------------------" << std::endl;
  std::cout << "diff21: (" << diff21.x << ", " << diff21.y << ")" << std::endl;
  std::cout << "prev_direction: " << prev_direction << std::endl;
  std::cout << "next_direction: " << next_direction << std::endl;
  */
  if (yidiff == 0) {
    float dir = prev_direction * next_direction;
    if (P1rx != P2rx && !float_is_zero (prev_direction) && !float_is_zero (next_direction) && dir > 0.f)
      bound.unset (P1ry, P1rx);
  } else if (std::abs(yidiff) == 1) {
    bound.change (P1ry, P1rx, (prev_point_disapears & bound(P1ry, P1rx)) ^ 1ULL);
    bound.change (P2ry, P2rx, (next_point_disapears & bound(P2ry, P2rx)) ^ 1ULL);
  } else {
    Dir2 x_diff = Dir2 (diff21.x / diff21.y, 0.f);
    if (P1.y > P2.y) {
      std::swap (P1r, P2r);
      std::swap (P1ry, P2ry);
      std::swap (P1rx, P2rx);
      std::swap (prev_point_disapears, next_point_disapears);
    }

    const uint64_t mid = (P2ry + P1ry) >> 1;
    Dir2 x = P1r;
    for (uint64_t yi = P1ry; yi <= mid; yi++) {
      uint64_t xi = static_cast<int64_t>(x.round().x);
      bound.change (yi, xi, (prev_point_disapears & bound(yi, xi)) ^ 1ULL);
      x += x_diff;
    }
      
    x = P2r;
    for (uint64_t yi = P2ry; yi > mid; yi--) {
      uint64_t xi = static_cast<int64_t>(x.round().x);
      bound.change (yi, xi, (next_point_disapears & bound(yi, xi)) ^ 1ULL);
      x -= x_diff;
    }
  }
}

enum BezierConfig {
  PrintDIR = 0b1,  // 1 for up
  TRule = 0b10,    // 1 for max
};

static void print_bezier_part (
  BoolMatrixU& bound, 
  const uint32_t bezier_config,
  const Dir2& av,
  const Dir2& bv,
  const Dir2& cv,
  const float& rem_sqrt,
  const float& iavy,
  const Dir2& first,
  int64_t last_height,
  float prev_direction,
  float next_direction,
  float curr_direction
) {
  float t = (bezier_config & BezierConfig::TRule ? 1.f : 0.f);
  uint64_t xi = static_cast<uint64_t> (first.round().x);
  const int64_t first_height = static_cast<int64_t> (first.round().y);
  const int64_t middle_height = (first_height + last_height) >> 1;
  
  /*
  std::cout << "prev_dis_cond: " << prev_direction << std::endl;
  std::cout << "next_dis_cond: " << next_direction << std::endl;
  std::cout << "curr_dis_cond: " << curr_direction << std::endl;
  */
  
  uint64_t prev_disappearance_cond = (prev_direction * curr_direction) < 0.f ? 1ULL : 0ULL;
  uint64_t next_disappearance_cond = (next_direction * curr_direction) < 0.f ? 1ULL : 0ULL;

  auto print_points = [&] (uint64_t yi, uint64_t yin, uint64_t disappearance_cond) {
    /*
    std::cout << "---------------------------------" << std::endl;
    std::cout << "(xi, yi): (" << xi << ", " << yi << ")" << std::endl;
    std::cout << "yin: " << yin << std::endl;
    std::cout << "disappearance_cond: " << disappearance_cond << std::endl;
    std::cout << "posicion: " << bound(yi, xi) << std::endl;
    std::cout << "need: " << ((disappearance_cond & bound(yi, xi)) ^ 1ULL)<< std::endl;
    std::cout << "t: " << t << std::endl;
    */
    bound.change (yi, xi, (disappearance_cond & bound(yi, xi)) ^ 1ULL);
    //std::cout << "posicion luego: " << bound(yi, xi) << std::endl;
    
    // test if there is a t for the height yi.
    float sqrt_cond = std::max (std::fmaf (av.y, static_cast<float>(yin), rem_sqrt), 0.f);
    sqrt_cond = std::sqrt (sqrt_cond);

    // calculate the yi.
    float t1 = iavy * (bv.y - sqrt_cond);
    bool print1 = (0.f < t1) && (t1 < 1.f);

    float t2 = iavy * (bv.y + sqrt_cond);
    bool print2 = (0.f < t2) && (t2 < 1.f);

    if (print1 && print2) {
      if (bezier_config & BezierConfig::TRule)
        t = std::max (t1, t2);
      else
        t = std::min (t1, t2);
    } else if (print1)
      t = t1;
    else if (print2)
      t = t2;

    xi = std::lround (av.madd(t, bv).madd(t, cv).x);
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
  /*
  std::cout << "nueva quad <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
  std::cout << "P1 = (" << P1.x << ", " << P1.y << ")"<< std::endl;
  std::cout << "P2 = (" << P2.x << ", " << P2.y << ")"<< std::endl;
  std::cout << "P3 = (" << P3.x << ", " << P3.y << ")"<< std::endl;
  */

  Dir2 P3r = P3.round();
  Dir2 P1r = P1.round();
  int64_t P3ry = static_cast<int64_t>(P3r.y);
  int64_t P1ry = static_cast<int64_t>(P1r.y);
  int64_t P3rx = static_cast<int64_t>(P3r.x);
  int64_t P1rx = static_cast<int64_t>(P1r.x);
  Dir2 av = P3 + P1 - P2 * 2.f;
  Dir2 bv = (P1 - P2).dir_mul(Dir2 {-2.f, 1.f});
  float rem_sqrt = P2.y * P2.y - P1.y * P3.y;
  float iavy = _mm_cvtss_f32 (_mm_permute_ps (_mm_rcp_ps (av.v), 0b01010101));

  float direction_calc_1 = P2.y - P1.y;
  if (float_is_zero (direction_calc_1))
    direction_calc_1 = P3.y - P1.y;

  float direction_calc_3 = P3.y - P2.y;
  if (float_is_zero (direction_calc_3)) 
    direction_calc_3 = P3.y - P1.y;
 
  /*
  std::cout << "prev_direction:" << prev_direction << std::endl;
  std::cout << "next_direction:" << next_direction << std::endl;
  std::cout << "direction_calc_1 :" << direction_calc_1 << std::endl;
  std::cout << "direction_calc_3 :" << direction_calc_3 << std::endl;
  */

  uint64_t prev_point_disapears = (direction_calc_1 * prev_direction < 0.f ? 1ULL : 0ULL);
  uint64_t next_point_disapears = (direction_calc_3 * next_direction < 0.f ? 1ULL : 0ULL);

  /*
  std::cout << "av.y :" << av.y << std::endl;
  std::cout << "bv.y :" << bv.y << std::endl;

  std::cout << "f1: " << float_is_zero ((P1 - P2).y) << std::endl; 
  std::cout << "f2: " << float_is_zero ((P3 - P1).y) << std::endl; 
  std::cout << "f3: " << ((P1 - P2).dir_mul(P3 - P2).y > 0.f) << std::endl; 
  */

  // cases.
  if (float_is_zero (av.y)) {
    if (float_is_zero (bv.y) || (P1.round() - P3.round()).y == 0.f) {
      draw_line (bound, P1, P3, prev_direction, next_direction);
    } else {
      if (P1.y > P3.y) {
        std::swap (prev_point_disapears, next_point_disapears);
        std::swap (P1ry, P3ry);
        std::swap (P1rx, P3rx);
      }

      int64_t mid = (P1ry + P3ry) >> 1;
      uint64_t xi = P1rx;
      for (int64_t yi = P1ry; yi <= mid; yi++) {
        /*
        std::cout << "(xi, yi): (" << xi << ", " << yi << ")" << std::endl;
        std::cout << "prev_point_disapears: " << prev_point_disapears << std::endl;
        std::cout << "bound(yi, xi): " << bound(yi, xi) << std::endl;
        */
        bound.change (yi, xi, (prev_point_disapears & bound(yi, xi)) ^ 1ULL);
        float t = 0.5f * (P1.y - static_cast<float>(yi + 1)) / bv.y;
        //std::cout << "t: " << t << std::endl;
        xi = static_cast<uint64_t>(av.madd(t, bv).madd(t, P1).round().x);
      }

      xi = P3rx;
      for (int64_t yi = P3ry; yi > mid; yi--) {
        /*
        std::cout << "(xi, yi): (" << xi << ", " << yi << ")" << std::endl;
        std::cout << "next_point_disapears: " << next_point_disapears << std::endl;
        std::cout << "bound(yi, xi): " << bound(yi, xi) << std::endl;
        */
        bound.change (yi, xi, (next_point_disapears & bound(yi, xi)) ^ 1ULL);
        float t = 0.5f * (P1.y - static_cast<float>(yi - 1)) / bv.y;
        //std::cout << "t: " << t << std::endl;
        xi = static_cast<uint64_t>(av.madd(t, bv).madd(t, P1).round().x);
      }
    }

  } else if (!float_is_zero ((P1 - P2).y) && !float_is_zero ((P3 - P2).y) && (P1 - P2).dir_mul(P3 - P2).y > 0.f) {
    float t_bound = bv.y * iavy;
    uint64_t yim = static_cast<uint64_t>(av.madd(t_bound, bv * -2.f).madd(t_bound, P1).round().y);
    uint64_t bc1 = 0, bc2 = BezierConfig::TRule;

    if ((P3 - P2).y < 0.f) {
      bc1 |= BezierConfig::PrintDIR;
      bc2 |= BezierConfig::PrintDIR;
    }
    print_bezier_part (
      bound, bc1, av, bv, P1, rem_sqrt, iavy, P1, yim, prev_direction, direction_calc_3, direction_calc_1
    );
    //std::cout << "||||||||||||||||||||-->>"<<  std::endl;
    print_bezier_part (
      bound, bc2, av, bv, P1, rem_sqrt, iavy, P3, yim, next_direction, direction_calc_1, direction_calc_3
    );

    //std::cout << "fin 2" << std::endl;
    return;
  } else {
    //std::cout << "3" << std::endl;

    uint64_t yim = (P1ry + P3ry) >> 1;
    Dir2 cond = P3.round() - P1.round();
    /*
    std::cout << "redondeo de p3y: " << P3.round().y << std::endl;
    std::cout << "redondeo de p1y: " << P1.round().y << std::endl;
    std::cout << "esto tiene una diferencia de: " << cond.y << std::endl;
    */

    if (cond.y == 0.f) {
      if (cond.x != 0.f && prev_direction * next_direction > 0.f)
        bound.unset (P1ry, P1rx);
    } else if (cond.abs().y == 1.f) {
      bound.change (P1ry, P1rx, (prev_point_disapears & bound(P1ry, P1rx)) ^ 1ULL);
      bound.change (P3ry, P3rx, (next_point_disapears & bound(P3ry, P3rx)) ^ 1ULL);
    } else if (0.f < cond.y) {
      //std::cout << "a" << std::endl;
      print_bezier_part (
        bound, BezierConfig::PrintDIR, 
        av, bv, P1, rem_sqrt, iavy, P1, yim, prev_direction, direction_calc_3, direction_calc_1
      );
      //std::cout << "||||||||||||||||||||-->>"<<  std::endl;
      print_bezier_part (
        bound, BezierConfig::TRule, 
        av, bv, P1, rem_sqrt, iavy, P3, yim+1, next_direction, direction_calc_1, direction_calc_3
      );
      //std::cout << "<<--||||||||||||||||||||"<<  std::endl;
    } else if (cond.y < 0.f) {
      //std::cout << "b" << std::endl;
      print_bezier_part (
        bound, 0, 
        av, bv, P1, rem_sqrt, iavy, P1, yim, prev_direction, direction_calc_3, direction_calc_1
      );
      //std::cout << "||||||||||||||||||||-->>"<<  std::endl;
      print_bezier_part (
        bound, BezierConfig::TRule | BezierConfig::PrintDIR, 
        av, bv, P1, rem_sqrt, iavy, P3, yim-1, next_direction, direction_calc_1, direction_calc_3
      );
      //std::cout << "<<--||||||||||||||||||||"<<  std::endl;
    }
    //std::cout << "fin 3" << std::endl;
  }
}


SDL_Surface* raster_grade_2 (const std::vector<std::vector<ComponentElement>>& components, SDL_Color color, AntiAliasingType antialias) {
  float antialias_multiplier;
  switch (antialias) {
    case AAx2: antialias_multiplier = 2.f; break;
    case AAx4: antialias_multiplier = 4.f; break;
    case AAx8: antialias_multiplier = 8.f; break;
    case AAx16: antialias_multiplier = 16.f; break;
    default: antialias_multiplier = 1.f;
  }

  Dir2 min = components[0][0].start, max = components[0][0].start;
  for (const auto& comp: components) {
    for (const auto& elems: comp) {
      min.v = _mm_min_ps (min.v, elems.start.v);
      max.v = _mm_max_ps (max.v, elems.start.v);

      min.v = _mm_min_ps (min.v, elems.end.v);
      max.v = _mm_max_ps (max.v, elems.end.v);

      if (elems.t == ComponentElementType::CURVE) {
        min.v = _mm_min_ps (min.v, elems.middle.v);
        max.v = _mm_max_ps (max.v, elems.middle.v);
      }
    }
  }

  // calculating dimensions of the image and matrix.
  Dir2 dims_l = max - min;
  Dir2 dims = dims_l.madd (antialias_multiplier, Dir2{16.f, 16.f});

  // creating the matrix for the bounds.
  int32_t matrix_height = std::lround (dims.y) + 2;
  int32_t matrix_width = std::lround (dims.x) + 2;
  BoolMatrixU bound = BoolMatrixU (matrix_height, matrix_width);

  // drawing.
  for (const auto& elements: components) {

    /* filtering problematic forms. The cases considering for evaluation are:
     *  1 - the first and last elements round to the same pixel 
     *        -> the curve will be eliminated.
     *  2 - its a curve, and the first and last elements are at the same hieght, 
     *      and the point with 0 derivative is also at that one
     *        -> becomes a horizontal line
     *  3 - its a horizontal line, and the previous one is also an horizontal line
     *        -> both becomes one
     * */

    std::vector<ComponentElement> filtered_elements;
    filtered_elements.reserve (elements.size ());
    bool last_component_was_hl = false;
    for (const auto& elem: elements) {
      const Dir2 P1 = elem.start * antialias_multiplier;
      const Dir2 P2 = elem.middle * antialias_multiplier;
      const Dir2 P3 = elem.end * antialias_multiplier;
      const Dir2 diff31r = P3.round () - P1.round ();

      // rule 1.
      if (diff31r == Dir2 {})
        continue;

      // rule 2.
      bool this_component_is_hl = diff31r.y == 0.f;
      if (elem.t == ComponentElementType::CURVE) {
        Dir2 av = P3 + P1 - P2 * 2.f;
        Dir2 bv = P1 - P2;
        float t_bound = bv.y * _mm_cvtss_f32 (_mm_permute_ps (_mm_rcp_ps (av.v), 0b01010101));
        float yim = av.madd(t_bound, bv * -2.f).madd(t_bound, P1).round().y;
        this_component_is_hl = P1.round().y == yim && P3.round().y == yim;
      }

      // rule 3.
      if (this_component_is_hl && last_component_was_hl) {
        filtered_elements.back().end = P3;
      } else if (this_component_is_hl) {
        filtered_elements.push_back (ComponentElement {
          .start = P1,
          .middle = Dir2 {},
          .end = P3,
          .t = ComponentElementType::LINE
        });
      } else {
        filtered_elements.push_back (ComponentElement {
          .start = P1,
          .middle = P2,
          .end = P3,
          .t = elem.t
        });

      }

      last_component_was_hl = this_component_is_hl;
    }

    // last iteration for the last and first elements.
    const auto last_elem = filtered_elements.back ();
    auto& first_elem = filtered_elements[0];
    const float diff = (first_elem.start.round () - last_elem.start.round ()).y;
    if (is_horizontal_line (first_elem) && is_horizontal_line (last_elem) && diff == 0.f) {
      first_elem.start = last_elem.start;
      filtered_elements.pop_back ();
    }

    // drawing the component inside the BoolMatrix.
    std::size_t pos = 0;
    std::size_t many = filtered_elements.size ();
    float prev_direction = get_prev_direction(filtered_elements.back ());
    float next_direction = get_next_direction(filtered_elements [1]);
    for (const auto& elem: filtered_elements) {
      switch (elem.t) {
        case ComponentElementType::CURVE:
          draw_quad_bezier (
            bound, 
            elem.start, 
            elem.middle, 
            elem.end, 
            prev_direction, 
            next_direction
          );
          break;
        case ComponentElementType::LINE:
          draw_line (
            bound, 
            elem.start, 
            elem.end, 
            prev_direction, 
            next_direction
          );
          break;
      }
      pos++;
      prev_direction = get_prev_direction(filtered_elements [pos - 1]);
      next_direction = get_next_direction(filtered_elements [(pos + 1) % many]);
    }
  }

  // creating the image filled with the data.
  const uint32_t height = std::lround(dims_l.y) + 1;
  const uint32_t width = std::lround(dims_l.x) + 1;
  SDL_Surface* surface = SDL_CreateRGBSurface (0, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  Uint32* pixels = (Uint32*)(surface->pixels), pixel_color;
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
