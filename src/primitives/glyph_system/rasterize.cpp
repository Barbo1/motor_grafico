#include "../../../headers/primitives/glyph_system.hpp"
#include "../../../headers/primitives/types_definition.hpp"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <cstdint>
#include <utility>
#include <iostream>
#include <variant>

static Dir2 resposition (const Dir2& vec, const Dir2& min, const Dir2& dims_resposition, const float& sizef) {
  Dir2 p = (vec - min) * sizef;
  p.y = (dims_resposition - p).y;
  return p;
}

static Dir2 get_mn (Dir2 m1, Dir2 m2) {
  float abs_a = std::abs (m1.x);
  float abs_b = std::abs (m2.x);
  float abs_c = std::abs (m1.y);
  float abs_d = std::abs (m2.y);
  float m_0 = std::max (abs_a, abs_b);
  float n_0 = std::max (abs_c, abs_d);
  return Dir2 (
    (std::abs(abs_a - abs_c) <= 33.f / 65536.f ? 2.f : 1.f) * m_0,
    (std::abs(abs_b - abs_d) <= 33.f / 65536.f ? 2.f : 1.f) * n_0  
  );
}

using pointSet = std::vector<
  std::tuple<
    std::vector<std::vector<std::pair<Dir2, uint8_t>>>, // points.
    std::vector<uint16_t> // offsets.
  >
>;

static pointSet get_points_set (
  uint16_t glyph_index,
  const std::vector<ttf_glyph_data>& glyphs, 
  std::vector<uint16_t>& index_acc,
  float inv_units_per_em_f,
  uint32_t max_points,
  int32_t depth
) {
  if (glyphs.size() <= glyph_index) {
    std::cout << "Font Error: the index '" << glyph_index << "' of the glyph is out of the table." << std::endl; 
    std::exit (-1);
  }

  if (depth < 0) {
    std::cout << "Font Error: the needed recursion level form compound glyph overpass the permited." << std::endl; 
    std::exit (-1);
  }

  pointSet points_set;
  if (std::holds_alternative<ttf_glyph_simple_data>(glyphs[glyph_index].raster_information)) {
    const ttf_glyph_simple_data& data = std::get<ttf_glyph_simple_data>(glyphs[glyph_index].raster_information);

    if (data.offset.size() == 0) {
      return points_set;
    } else if (data.offset.back() > max_points) {
      std::cout 
        << "Font Error: the number of points in the simple glyph are greater than '" 
        << max_points 
        << "'." 
        << std::endl; 
      std::exit (-1);
    }

    std::vector<std::vector<std::pair<Dir2, uint8_t>>> points (data.offset.size ());

    // generating array of the point of the scaled glyph.
    uint32_t curr_off = 0, pos = 0;
    for (auto& off: data.offset) {
      uint32_t many = off - curr_off;
      points[pos].resize (many + 4);

      for (uint32_t i = 0; i <= many; i++) {
        points[pos][i] = std::pair<Dir2, uint8_t> (
          data.points[i + curr_off], 
          data.flags[i + curr_off] & 1
        );
      }

      points[pos][many+1] = points[pos][0];
      points[pos][many+2] = points[pos][1];
      points[pos][many+3] = points[pos][2];

      curr_off = off + 1;
      pos++;
    }

    points_set.push_back(std::make_tuple(points, data.offset));
    return points_set;
  } else {
    const ttf_glyph_compound_data& data = std::get<ttf_glyph_compound_data>(glyphs[glyph_index].raster_information);

    // getting the points of all the previous glyphs.
    for (const auto& index: data.components) {
      if (std::find (index_acc.begin(), index_acc.end(), index) != index_acc.end()) {
        std::cout << "Font Error: font metadata ill-formed for a compound bringing an auto-reference." << std::endl; 
        std::exit (-1);
      }
      index_acc.push_back(index);

      pointSet aux = get_points_set (index, glyphs, index_acc, inv_units_per_em_f, max_points, depth-1);
      points_set.insert (points_set.end(), aux.cbegin(), aux.cend());
    }
    
    // deduce reposition points.
    std::vector<Dir2> repos_points;
    for (uint32_t pos = 0; pos < points_set.size(); pos++) {
      const auto& glyph = points_set[pos];
      const auto& trans = data.tranformation[pos];

      // (cases needs revision)
      if (data.flags[pos] & 0b10) {
        // std::cout << "caso 1." << std::endl;
        repos_points.push_back(Dir2 (
          static_cast<float>(std::bit_cast<int16_t>(trans.first[0])), 
          static_cast<float>(std::bit_cast<int16_t>(trans.first[1]))
        ) * inv_units_per_em_f);
      } else {
        // std::cout << "caso 2." << std::endl;
        uint16_t index1 = trans.first[0];
        uint16_t index2 = trans.first[1];
        Dir2 pp = Dir2 {};
        Dir2 pc = Dir2 {};

        // obtain parent point.
        uint16_t off_pos = 0;
        uint16_t off_acc = 0;
        for (const auto& glyph_to_find: points_set) {
          const std::vector<uint16_t>& offs = std::get<1>(glyph_to_find);
          uint32_t i = 0;
          while (i < offs.size() && index1 < offs[i] + off_acc) { i++; }
          if (i == offs.size()) {
            off_acc += offs.back();
          } else {
            pp = std::get<0>(glyph_to_find)[off_pos][i].first;
            break;
          }
          off_pos++;
        }

        if (off_pos == points_set.size()) {
          std::cout << "Font Error: font metadata ill-formed for a compound glyph giving ill-formed index(1)." << std::endl; 
          std::exit (-1);
        }

        // obtain child point.
        const std::vector<uint16_t>& offs = std::get<1>(glyph);
        uint32_t i = 0;
        while (i < offs.size() && index2 < offs[i]) { i++; }
        if (i == offs.size()) {
          std::cout << "Font Error: font metadata ill-formed for a compound glyph giving ill-formed index(2)." << std::endl; 
          std::exit (-1);
        } else {
          pc = std::get<0>(glyph)[off_pos][i].first;
        }
        
        Dir2 m1 = Dir2 (trans.second[0], trans.second[2]);
        Dir2 m2 = Dir2 (trans.second[1], trans.second[3]);
        Dir2 r = Dir2 (m1 * pc, m2 * pc);
        repos_points.push_back(pp - r);
      }
    }

    // transform given points.
    for (uint32_t pos = 0; pos < points_set.size(); pos++) {
      auto& glyph = points_set[pos];
      const auto& trans = data.tranformation[pos];
      Dir2 m1 = Dir2 (trans.second[0], trans.second[2]);
      Dir2 m2 = Dir2 (trans.second[1], trans.second[3]);
      Dir2 off = repos_points[pos].dir_mul(get_mn (m1, m2));

      for (auto& line: std::get<0>(glyph)) {
        for (auto& point: line) {
          point.first = Dir2 (m1 * point.first, m2 * point.first) + off;
        }
      }
    }

    return points_set;
  }
}

SDL_Surface* GlyphsSystem::raster (char16_t character, uint32_t s) {
  auto founded = this->mapping.find (character);
  if (founded == this->mapping.end())
    return SDL_CreateRGBSurface (
      0, 1, 1, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
    );

  float sizef = static_cast<float>(s);
  uint32_t glyph_index = founded->second;
  Dir2 min = this->glyphs[glyph_index].bounding_box.first - Dir2 (0.1f, 0.1f);
  Dir2 max = this->glyphs[glyph_index].bounding_box.second + Dir2 (0.1f, 0.1f);
  Dir2 dims_resposition = (max - min) * sizef;

  std::vector<std::vector<std::vector<ComponentElement>>> parts;
  
  {
    std::vector<uint16_t> index_acc;
    index_acc.push_back (glyph_index);
    pointSet point_set = get_points_set (
      glyph_index, 
      this->glyphs, 
      index_acc, 
      this->inv_units_per_em_f,
      this->max_points,
      this->max_component_depth
    );

    if (point_set.size() == 0) {
      return SDL_CreateRGBSurface (
        0, 1, 1, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
      );
    }

    for (const auto& comps: point_set) {
      const std::vector<uint16_t>& offset_set = std::get<1>(comps);

      // creating structure for the information of the image.
      parts.emplace_back(std::vector<std::vector<ComponentElement>>());
      auto& components = parts.back();
      components.resize(offset_set.size());

      std::size_t comp_pos = 0;
      for (const auto& vec: std::get<0>(comps)) {
        uint32_t pos = 0, next_pos = 1;
        uint32_t many = vec.size() - 4;

        Dir2 P1 = (vec[pos].second ? 
          Dir2{} : 
          (vec[many].second ? vec[many].first : (vec[many].first + vec[pos].first) * 0.5f)
        );
        Dir2 P2 = vec[pos].first;
        Dir2 P3 = vec[next_pos].first;

        // creating the elements list.
        auto& elements = components[comp_pos];
        elements.reserve (30);
        while (pos <= many) {
          switch ((vec[pos].second << 1) | vec[next_pos].second) {
            case 0b00: {
                Dir2 PA = (P3 + P2) * 0.5f;
                elements.emplace_back (ComponentElement {
                  .start = P1,
                  .middle = P2,
                  .end = PA,
                  .t = ComponentElementType::CURVE
                });
                P1 = PA;
              }
              break;
            case 0b01:
              elements.emplace_back (ComponentElement {
                .start = P1,
                .middle = P2,
                .end = P3,
                .t = ComponentElementType::CURVE
              });
              break;
            case 0b11:
              elements.emplace_back (ComponentElement {
                .start = P2,
                .middle = Dir2 {},
                .end = P3,
                .t = ComponentElementType::LINE
              });
            case 0b10:
              P1 = P2;
              break;
          }
          pos++; next_pos++;
          P2 = P3;
          P3 = vec[next_pos].first;
        }

        comp_pos++;
      }
    }
  }
  
  // mirror the letter in y axis.
  for (auto& glyph: parts) {
    for (auto& line: glyph) {
      for (auto& component: line) {
        component.start = resposition (component.start, min, dims_resposition, sizef);
        component.middle = resposition (component.middle, min, dims_resposition, sizef);
        component.end = resposition (component.end, min, dims_resposition, sizef);
      } 
    } 
  }

  // image creation.
  SDL_Surface* surface = nullptr;
  for (const auto& components: parts) {
    SDL_Surface* aux = raster_grade_2(
      components, 
      min * sizef, max * sizef, 
      SDL_Color {.r=255, .g=255, .b=255, .a=255}, 
      AntiAliasingType::AAx16
    );

    if (surface == nullptr) {
      surface = aux;
    } else {
      Uint32* dst = (Uint32*)surface->pixels;
      Uint32* src = (Uint32*)aux->pixels;
      uint32_t width = surface->w;
      for (uint32_t h = 0; h < (uint32_t)surface->h; h++) {
        for (uint32_t w = 0; w < width; w++) {
          dst[width * h + w] = std::max(dst[width * h + w], src[width * h + w]);
        }
      }
      SDL_FreeSurface(aux);
    }
  }

  if (surface == nullptr) {
    surface = SDL_CreateRGBSurface (
      0, 1, 1, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
    );
  }

  return surface;
}
