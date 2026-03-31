#include "../../headers/primitives/global.hpp"
#include "../../headers/primitives/vectors.hpp"
#include "../../headers/concepts/glyph_system.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <array>
#include <codecvt>
#include <iostream>
#include <cmath>

/*
template<std::size_t N>
uint32_t get_c (const std::array<Dir2, N>& points, const std::array<uint32_t, 3*(N-2)>& parts_indexes, uint32_t first, uint32_t last, bool clockwise) {
  std::vector<int32_t> ret = std::vector<int32_t>();
  
  Dir2 v1 = points[N-1] - points[0];
  Dir2 v2 = points[1] - points[0];
  if ((std::atan2(v1.percan() * v2, v1 * v2) > 0.f) ^ clockwise) {
    ret.push_back(0);
  }

  v1 = points[N-2] - points[N-1];
  v2 = points[0] - points[N-1];
  if ((std::atan2(v1.percan() * v2, v1 * v2) > 0.f) ^ clockwise) {
    ret.push_back(N-1);
  }

  for (uint32_t i = 1; i < N-1; i++) {
    Dir2 v1 = points[i-1] - points[i];
    Dir2 v2 = points[i+1] - points[i];
    if ((std::atan2(v1.percan() * v2, v1 * v2) > 0.f) ^ clockwise) {
      ret.push_back(i);
    }
  }

  ret.push_back(N);

  return ret;
}
*/


std::vector<std::array<Dir2, 3>> get_partitions (std::vector<Dir2> points) {
  std::vector<std::array<Dir2, 3>> parts = std::vector<std::array<Dir2, 3>>();
  parts.resize(points.size());

  Dir2 vp = points.back() - points[0];
  Dir2 vc = points[1] - points[0];

  points.push_back(points[0]);
  parts[0] = {points[0], vp, vc};
  for (uint32_t i = 1; i < points.size()-1; i++) {
    vp = -vc;
    vc = points[i+1] - points[i];
    parts[i] = {points[i], vp, vc};
  }
  return parts;
}

std::array<Dir2, 7> set_points_1 () {
  return std::array<Dir2, 7> {
    Dir2 (219.4f, 404.2f), Dir2 (394.0f, 361.3f), Dir2 (522.9f, 441.6f), 
    Dir2 (520.1f, 244.8f), Dir2 (437.0f, 91.0f), Dir2 (233.2f, 25.9f), 
    Dir2 (291.4f, 170.0f)
  };
}

std::array<Dir2, 11> set_points_2 () {
  return std::array<Dir2, 11> {
    Dir2 (315.6f, 41.4f), Dir2 (219.9f, 200.4f), 
    Dir2 (77.0f, 188.3f), Dir2 (55.5f, 313.6f), Dir2 (246.8f, 359.4f), 
    Dir2 (337.1f, 323.0f), Dir2 (364.1f, 273.2f), Dir2 (424.7f, 209.8f),
    Dir2 (528.5f, 316.3), Dir2 (593.2f, 228.7f), Dir2 (525.8f, 99.3f)
  };
}

std::array<Dir2, 17> set_points_3 () {
  return std::array<Dir2, 17> {
    Dir2 (350.0f, 256.8f), Dir2 (400.f, 200.f), 
    Dir2 (416.8f, 105.4f), Dir2 (485.2f, 147.7f), Dir2 (481.9f, 253.6f), 
    Dir2 (542.2f, 354.5f), Dir2 (625.2f, 256.8f), Dir2 (582.9f, 116.8f), 
    Dir2 (173.1f, 22.3f), Dir2 (34.1f, 159.1f), Dir2 (58.5f, 384.5f), 
    Dir2 (123.6f, 336.6f), Dir2 (113.9f, 279.6f), Dir2 (125.3f, 206.3f), 
    Dir2 (159.5f, 95.6f), Dir2 (226.2f, 103.7f), Dir2 (273.5f, 211.2f), 
  };
}

/*
  return std::array<Dir2, 17> {
     
    Dir2 (273.5f, 211.2f), 
    Dir2 (350.0f, 256.8f), Dir2 (400.f, 200.f), Dir2 (416.8f, 105.4f), 
    Dir2 (485.2f, 147.7f), Dir2 (481.9f, 253.6f), Dir2 (542.2f, 354.5f), 
    Dir2 (625.2f, 256.8f), Dir2 (582.9f, 116.8f), Dir2 (173.1f, 22.3f), 
    Dir2 (34.1f, 159.1f), Dir2 (58.5f, 384.5f), Dir2 (123.6f, 336.6f), 
    Dir2 (113.9f, 279.6f), Dir2 (125.3f, 206.3f),Dir2 (159.5f, 95.6f), Dir2 (226.2f, 103.7f), 
  };
 * */

/*
  return std::array<Dir2, 17> {
    Dir2 (113.9f, 279.6f), Dir2 (125.3f, 206.3f), 
    Dir2 (159.5f, 95.6f), Dir2 (226.2f, 103.7f), Dir2 (273.5f, 211.2f), 
    Dir2 (350.0f, 256.8f), Dir2 (400.f, 200.f), Dir2 (416.8f, 105.4f), 
    Dir2 (485.2f, 147.7f), Dir2 (481.9f, 253.6f), Dir2 (542.2f, 354.5f), 
    Dir2 (625.2f, 256.8f), Dir2 (582.9f, 116.8f), Dir2 (173.1f, 22.3f), 
    Dir2 (34.1f, 159.1f), Dir2 (58.5f, 384.5f),Dir2 (123.6f, 336.6f)
  };
 * */

std::u16string to_u16string(int32_t i) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
  return conv.from_bytes(std::to_string(i));
}

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});
  SDL_Event event;
  
  int32_t error;
  GlyphsSystem gs (glb, "../fuentes_letras/Nostard-Medium.ttf", &error);
  std::array<Dir2, 7> points = set_points_1();
  const uint32_t N = points.size();
  
  std::array<uint32_t, 3*N> parts_indexes;
  uint32_t total_parts = 1;

  {
    bool state_1 = true;

    // clock order.
    Dir2 mult = Dir2(-1.f, 1.f);
    Dir2 aux = points[N-1].dir_mul(mult) + points[0];
    float order = aux.y * aux.x;
    for (uint32_t i = 0; i < N-1; i++) {
      aux = points[i].dir_mul(mult) + points[i+1];
      order += aux.y * aux.x;
    }
    bool clockwise = order > 0.f;

    // structure definitions.
    std::array<std::pair<uint32_t, uint32_t>, N> parts_bounds;
    std::array<std::array<Dir2, 3>, N> parts;
    std::array<uint32_t, 3*N> aux_arr;
    uint32_t parts_bounds_size = 1;
    uint32_t parts_indexes_last = N-1;
    for (uint32_t i = 0; i < N; i++)
      parts_indexes[i] = i;
    parts_bounds[0] = {0, N-1};

    /* * * * * * * * * * * *
     *   Obtuse division   *
     * * * * * * * * * * * */
    while (true) {

      // finding a new non-triangle polygon.
      uint32_t ind = 0;
      uint32_t first, last;
      int32_t parts_size;
      int32_t index_l = 0, index_f = 0;
      bool angle_cond;
      if (state_1) {
        angle_cond = false;
        for (uint32_t i = 0; i < parts_bounds_size; i++) {
          first = parts_bounds[i].first;
          last = parts_bounds[i].second;

          // creating the array of parts.
          parts_size = last - first + 1;
          Dir2 v1 = points[parts_indexes[last]] - points[parts_indexes[first]];
          Dir2 v2 = points[parts_indexes[first + 1]] - points[parts_indexes[first]];
          parts[0] = {points[parts_indexes[first]], v1, v2};
          for (int32_t j = 1; j < parts_size-1; j++) {
            v1 = -v2;
            v2 = points[parts_indexes[first + j + 1]] - points[parts_indexes[first + j]];
            parts[j] = {points[parts_indexes[first + j]], v1, v2};
          }
          v1 = -v2;
          v2 = points[parts_indexes[first]] - points[parts_indexes[last]];
          parts[parts_size-1] = {points[parts_indexes[last]], v1, v2};

          for (int32_t j = 0; j < parts_size; j++) {
            Dir2 v1 = parts[j][1];
            Dir2 v2 = parts[j][2];
            if ((std::atan2(v1.percan() * v2, v1 * v2) > 0.f) ^ clockwise) {
              ind = i;
              index_f = j;
              goto POLY_OBT_DIV_FIN;
            }
          }
        }
        state_1 = false;
        continue;
        POLY_OBT_DIV_FIN:
      } else {
        for (uint32_t i = 0; i < parts_bounds_size; i++) {
          first = parts_bounds[i].first;
          last = parts_bounds[i].second;
          if (last-first > 2) {
            ind = i;
            goto POLY_STATE_2_END_LOOP;
          }
        }
        break;
        POLY_STATE_2_END_LOOP:

        // creating the array of parts.
        parts_size = last - first + 1;
        Dir2 v1 = points[parts_indexes[last]] - points[parts_indexes[first]];
        Dir2 v2 = points[parts_indexes[first + 1]] - points[parts_indexes[first]];
        parts[0] = {points[parts_indexes[first]], v1, v2};
        for (int32_t i = 1; i < parts_size-1; i++) {
          v1 = -v2;
          v2 = points[parts_indexes[first + i + 1]] - points[parts_indexes[first + i]];
          parts[i] = {points[parts_indexes[first + i]], v1, v2};
        }
        v1 = -v2;
        v2 = points[parts_indexes[first]] - points[parts_indexes[last]];
        parts[parts_size-1] = {points[parts_indexes[last]], v1, v2};

        // finding point with greater angle.
        float distance = INFINITY;
        index_l = 0, index_f = 0;
        for (int32_t i = 0; i < parts_size; i++) {
          const Dir2 vp = parts[i][1];
          const Dir2 vc = parts[i][2];
          const float new_dist = (vp * vc) / (vp.modulo() * vc.modulo());
          if ((new_dist > distance) ^ clockwise) {
            angle_cond = (std::atan2(vc.percan() * vp, vp * vc) > 0.f) ^ clockwise;
            distance = new_dist;
            index_f = i;
          }
        }
      }

      // Finding point of division.
      float distance = INFINITY, new_dist;
      std::array<Dir2, 3> part = parts[index_f];
      Dir2 given_point = part[0];
      Dir2 v1 = part[1];
      Dir2 v2 = part[2];
      Dir2 vn = (v1.normalize() + v2.normalize()).normalize().percan();
      for (int32_t i = 0; i < parts_size; i++) {
        Dir2 vec_new_try = parts[i][0] - given_point;
        float c1 = v1.pLd(vec_new_try, v2);
        float c2 = v2.pLd(vec_new_try, v1);
        int32_t diffi = std::abs (index_f - i);
        if (diffi < 2 || diffi == parts_size-1 || ((c1 > 0.0001f && c2 > 0.0001f) ^ angle_cond))
          continue;

        for (int32_t j = 0; j < parts_size; j++) {
          int32_t diffi = std::abs (index_f - j);
          if (diffi < 2 || diffi == parts_size-1)
            continue;
          Dir2 vec_line = parts[j][2];
          Dir2 diff = parts[j][0] - given_point;

          float c1 = vec_line.pLd(diff, vec_new_try);
          float c2 = -vec_new_try.pLd(diff, vec_line);
          if ((0.0001f < c1 && c1 < 0.9998f && 0.0001f < c2 && c2 < 0.9998f)) {
            goto FIN_DIST_PLOY;
          }
        }

        new_dist = (vn * (vn * vec_new_try)).modulo();
        if (new_dist < distance) {
          distance = new_dist;
          index_l = i;
        }

        FIN_DIST_PLOY:
      }

      if (distance == INFINITY) {
        std::cout << "pasa algo" << std::endl;
        std::exit(-1);
      }

      std::pair<uint32_t, uint32_t> max_min = std::minmax(index_l, index_f);
      index_f = max_min.first + first;
      index_l = max_min.second + first;

      // Generating division
        // Changing boundings.
      for (uint32_t i = 0; i < parts_bounds_size; i++) {
        if (parts_bounds[i].first > first) {
          parts_bounds[i].first += 2;
          parts_bounds[i].second += 2;
        }
      }

      uint32_t elem1 = index_l - index_f + 1;
      uint32_t elem2 = last - elem1 + 3;
      parts_bounds[parts_bounds_size++] = {elem2, parts_bounds[ind].second + 2};
      parts_bounds[ind].second = elem2-1;

        // Changing indexes.
      uint32_t* indexp = parts_indexes.data();
      uint32_t* auxp = aux_arr.data();
      uint32_t* indexpa = indexp + last;
      uint32_t* indexpf = indexp + index_f;
      uint32_t* indexpl = indexp + index_l;
      std::memmove(indexpa + 3, indexpa + 1, sizeof(uint32_t) * (parts_indexes_last - last + 1));
      std::memcpy(auxp, indexpf, sizeof(uint32_t) * elem1);
      std::memmove(indexpf + 1, indexpl, sizeof(uint32_t) * (last - index_l + 1));
      std::memcpy(indexp + elem2, auxp, sizeof(uint32_t) * elem1);
      parts_indexes_last += 2;
      total_parts++;
    }
  }


  bool cont = true;
  while (cont) {
    glb->begin_render();

    SDL_SetRenderDrawColor(glb->get_render(), 255, 0, 0, 255);
    for (uint32_t i = 0; i < total_parts; i++) {
      Dir2 point1 = points[parts_indexes[3*i]];
      Dir2 point2 = points[parts_indexes[3*i + 1]];
      Dir2 point3 = points[parts_indexes[3*i + 2]];
      SDL_RenderDrawLine (
        glb->get_render(), 
        static_cast<uint32_t>(point1.x),
        static_cast<uint32_t>(point1.y),
        static_cast<uint32_t>(point2.x),
        static_cast<uint32_t>(point2.y)
      );
      SDL_RenderDrawLine (
        glb->get_render(), 
        static_cast<uint32_t>(point1.x),
        static_cast<uint32_t>(point1.y),
        static_cast<uint32_t>(point3.x),
        static_cast<uint32_t>(point3.y)
      );
      SDL_RenderDrawLine (
        glb->get_render(), 
        static_cast<uint32_t>(point2.x),
        static_cast<uint32_t>(point2.y),
        static_cast<uint32_t>(point3.x),
        static_cast<uint32_t>(point3.y)
      );
    }

    for (uint32_t i = 0; i < points.size(); i++) {
      const Dir2& point = points[i];
      gs.print(to_u16string(i), 20, SDL_Color {.r = 255, .g = 255, .b = 255, .a = 255}, point + Dir2(5.f, 5.f));
    }

    /* Evaluacion de perifericos. */
    if (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          cont = false;
          break;

        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
              cont = false;
              break;
          }
          break;
      }
    }

    glb->end_render();
  } 
}
