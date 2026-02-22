#include "../../../headers/primitives/glyph_system.hpp"
#include "../../../headers/primitives/types_definition.hpp"
#include "../../../headers/primitives/rasterizer.hpp"
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <utility>

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

  // generating array of the point of the scaled glyph.
  std::vector<std::vector<std::pair<Dir2, uint8_t>>> points (data.offset.size ());
  uint32_t curr_off = 0, pos = 0;
  for (auto& off: data.offset) {
    uint32_t many = off - curr_off;
    points[pos].resize (many + 4);

    for (uint32_t i = 0; i <= many; i++) {
      Dir2 p = (data.points[i + curr_off] - min) * sizef;
      p.y = (dims_reposition - p).y;
      points[pos][i] = std::pair<Dir2, uint8_t> (p, data.flags[i + curr_off] & 1);
    }

    points[pos][many+1] = points[pos][0];
    points[pos][many+2] = points[pos][1];
    points[pos][many+3] = points[pos][2];

    curr_off = off + 1;
    pos++;
  }

  // creating structure for the information of the image.
  std::vector<std::vector<ComponentElement>> components(data.offset.size());
  std::size_t comp_pos = 0;
  std::size_t elem_count = 0;
  for (const auto& vec: points) {
    uint32_t pos = 0, next_pos = 1;
    uint32_t many = vec.size() - 4;

    Dir2 P1 = (vec[pos].second ? 
      Dir2{} : 
      (vec[many].second ? vec[many].first : (vec[many].first + vec[pos].first) * 0.5f)
    );
    Dir2 P2 = vec[pos].first;
    Dir2 P3 = vec[next_pos].first;
    Dir2 PA = Dir2 {};

    // creating the elements list.
    auto& elements = components[comp_pos];
    elements.reserve (30);
    while (pos <= many) {
      switch ((vec[pos].second << 1) | vec[next_pos].second) {
        case 0b00:
          PA = (P3 + P2) * 0.5f;
          elements.push_back (ComponentElement {
            .start = P1,
            .middle = P2,
            .end = PA,
            .t = ComponentElementType::CURVE
          });
          elem_count++;
          P1 = PA;
          break;
        case 0b01:
          elements.push_back (ComponentElement {
            .start = P1,
            .middle = P2,
            .end = P3,
            .t = ComponentElementType::CURVE
          });
          elem_count++;
          break;
        case 0b11:
          elements.push_back (ComponentElement {
            .start = P2,
            .middle = Dir2 {},
            .end = P3,
            .t = ComponentElementType::LINE
          });
          elem_count++;
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

  if (elem_count != 0)
    return raster_grade_2(components, SDL_Color {.r=255, .g=255, .b=255, .a=255}, AntiAliasingType::AAx16);
  else {
    Dir2 dims = dims_reposition.round();
    return SDL_CreateRGBSurface (
      0, static_cast<uint64_t>(dims.x), static_cast<uint64_t>(dims.y), 32, 
      0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
    );
  }
}
