#include "../../../../headers/primitives/glyph_system.hpp"
#include "../../../../headers/primitives/file_processing.hpp"
#include <bit>
#include <cstdint>
#include <iostream>

GlyphsSystem::GlyphsSystem (Global* glb, std::string path, int* error) {
  SequentialFileReader file(path, error);
  if (*error != 0) {
    std::cout << "Font Error: file not found" << std::endl;
    return ;
  }

  uint32_t value = file.read32();
  if (value != 0x00010000 && value != 0x74727565) {
    std::cout << "Font Error: scaler mismatch" << std::endl;
    return ;
  }

  uint16_t number_tables = file.read16();
  file.read16(); file.read16(); file.read16();

  std::vector<ttf_table_info> tables_info;

  for (uint32_t i = 0; i < number_tables; i++) {
    uint32_t tag = file.read32();
    tables_info.push_back (ttf_table_info {
      .tag = tag,
      .checksum = file.read32(),
      .offset = file.read32(),
      .length = file.read32()
    });
  }

  std::vector<ttf_table_info>::iterator readedit = tables_info.begin();
  std::array<uint32_t, 9>::const_iterator requiredit = required_tags.begin();

  /* Evaluate if all needed are present. */
  while (readedit != tables_info.end() && requiredit != required_tags.end()) {
    if (*requiredit == (*readedit).tag) {
      requiredit++;
    }
    readedit++;
  }

  if (requiredit != required_tags.end()) {
    std::cout << "Font Error: required tag " << *requiredit
      << " not found or the tags are not in the proper order." 
      << std::endl;
    return;
  }

  /* Ordering all the tags as needed. */
  for (uint32_t i = 0; i < tags_order.size(); i++) {
    uint32_t j = i + 1;
    while (j < tables_info.size() && tags_order[i] != tables_info[j].tag) j++;
    if (j < tables_info.size())
      std::swap(tables_info[j], tables_info[i]);
  }

  this->mapping = std::map<char16_t, uint16_t> ();
  this->cached_glyphs = std::map<uint32_t, TTFCachedGlyphInfo> ();
  this->glyphs = std::vector<ttf_glyph_data> ();
  this->glb = glb;

  /*****************/
  /*  readin head  */
  /*****************/

  file.set_file_position (tables_info[0].offset);
  if (file.read32 () != 0x00010000) {
    std::cout << "Font Error: head version not correct." << std::endl;
    return;
  }
  file.read32 ();
  file.read32 (); // checksum.
  if (file.read32 () != 0x5F0F3CF5) {
    std::cout << "Font Error: magic number not correct." << std::endl;
    return;
  }

  uint16_t head_flags = file.read16 ();
  if (head_flags & 0x0040) {
    std::cout << "Font Error: bit must be 0, but 1 found." << std::endl;
    return;
  }

  uint16_t units_per_em = file.read16();
  float inv_units_per_em_f = 1.f / static_cast<float>(units_per_em);
  file.read64(); file.read64();
  uint16_t x_min = file.read16 ();
  uint16_t y_min = file.read16 ();
  uint16_t x_max = file.read16 ();
  uint16_t y_max = file.read16 ();
  uint16_t mac_style = file.read16 ();
  uint16_t lowest_rec_ppem = file.read16 ();
  int16_t font_direction_hint = std::bit_cast<int16_t> (file.read16 ());
  int16_t index_to_loc_format = std::bit_cast<int16_t> (file.read16 ());

  if (std::bit_cast<int16_t> (file.read16 ()) != 0) {
    std::cout << "Font Error: glyphDataFromat is not 0." << std::endl;
    return;
  }

  /* eliminar esta basofia. */
  x_max = x_min + y_min + x_max + y_max + mac_style + lowest_rec_ppem + font_direction_hint + units_per_em; 


  /*****************/
  /*  readin maxp  */
  /*****************/


  file.set_file_position (tables_info[1].offset);
  if (file.read32 () != 0x00010000) {
    std::cout << "Font Error: maxp version not correct." << std::endl;
    return;
  }
  uint16_t number_glyphs = file.read16 ();


  /*****************/
  /*  readin cmap  */
  /*****************/


  file.set_file_position (tables_info[2].offset);
  if (file.read16 () != 0) {
    std::cout << "Font Error: version not matching required." << std::endl;
    return;
  }

  uint16_t subtables = file.read16();
  bool founded = true;
  uint32_t offset;
  while (founded && subtables--) {
    uint16_t PID = file.read16 ();
    uint16_t PSID = file.read16 ();
    offset = file.read32 ();
    founded = !((PID == 0 && PSID == 3) || (PID == 3 && PSID == 1));
  }

  if (!founded) {
    file.set_file_position (tables_info[2].offset + offset);
    if (file.read16() != 4) {
      std::cout << "Font Error: not founded a subtable of format 4." << std::endl;
      return;
    }

    file.read32 ();
    uint16_t seg_count_2 = file.read16 ();
    uint32_t seg_count = seg_count_2 >> 1;
    file.read32 (); file.read16 ();

    std::vector<uint16_t> start_code = std::vector<uint16_t>(seg_count);
    std::vector<uint16_t> end_code = std::vector<uint16_t>(seg_count);
    std::vector<int16_t> id_delta = std::vector<int16_t>(seg_count);
    std::vector<uint16_t> id_range_offset = std::vector<uint16_t>(seg_count);
    std::vector<uint16_t> glyph_index_array = std::vector<uint16_t>();

    for (uint32_t i = 0; i < seg_count; i++)
      end_code[i] = file.read16 ();
    file.read16 ();
    for (uint32_t i = 0; i < seg_count; i++)
      start_code[i] = file.read16 ();
    for (uint32_t i = 0; i < seg_count; i++)
      id_delta[i] = std::bit_cast<int16_t>(file.read16 ());
    for (uint32_t i = 0; i < seg_count; i++)
      id_range_offset[i] = file.read16 ();

    if (start_code[seg_count - 1] != end_code[seg_count - 1] || start_code[seg_count - 1] != 0xFFFF) {
      std::cout << "Font Error: there is no consistency in subtable inforamtion." << std::endl;
      return;
    }

    while (file.get_file_position() < tables_info[2].length + tables_info[2].offset) {
      glyph_index_array.push_back (file.read16());
    }

    for (uint32_t i = 0; i < seg_count - 1; i++) {
      if (id_range_offset[i] == 0)
        for (uint16_t j = start_code[i]; j <= end_code[i]; j++)
          this->mapping.insert ({j, j + id_delta[i] + 1});
      else
        for (uint16_t j = start_code[i], dev = 0; j <= end_code[i]; j++, dev++) {
          uint16_t pos = id_range_offset [i] + dev + i - seg_count;
          if (glyph_index_array.size() <= pos) {
            std::cout << "Font Error: glyphIndexAddress pass the allowed." << std::endl;
            return;
          }

          uint16_t elem = glyph_index_array[pos];
          elem = elem != 0 ? (elem + id_delta[i]) % 65536 : 0;
          this->mapping.insert ({j, elem + 1});
        }
    }
  } else {
    std::cout << "Font Error: not founded a subtable of format 4." << std::endl;
    return;
  }


  /*****************/
  /*  readin loca  */
  /*****************/
  
  
  file.set_file_position (tables_info[3].offset);
  std::vector<uint32_t> loca_offsets (number_glyphs);

  if (index_to_loc_format == 0) {
    for (uint32_t i = 0; i < (uint32_t)(number_glyphs + 1); i++) {
      loca_offsets[i] = 2 * file.read16();
    }
  } else {
    for (uint32_t i = 0; i < (uint32_t)(number_glyphs + 1); i++) {
      loca_offsets[i] = file.read32();
    }
  }


  /*****************/
  /*  readin glyf  */
  /*****************/
 

  this->mapping.insert ({0, 0});
  this->cached_glyphs.insert ({0, TTFCachedGlyphInfo {
    SDL_CreateTexture (
      glb->get_render(), 
      SDL_PIXELFORMAT_RGBA8888, 
      SDL_TEXTUREACCESS_STATIC, 
      1, 
      1
    ), 0, 0
  }});
  this->glyphs.resize (number_glyphs + 1);
  this->glyphs[0] = ttf_glyph_data {
    .raster_information = ttf_glyph_simple_data { 
      .points = std::vector<Dir2>(), 
      .flags = std::vector<uint8_t> (),
      .offset = std::vector<uint16_t> ()
    }, 
    .bounding_box = {Dir2 {1.f, 1.f}, Dir2 {1.f, 1.f}},
    .left_bearing = 1.f,
    .contours = 0
  };
 
  for (uint32_t loff = 0; loff < loca_offsets.size () - 1; loff++) {
    file.set_file_position (tables_info[4].offset + loca_offsets[loff]);

    uint32_t length_glyph = loca_offsets[loff+1] != loca_offsets[loff];
    if (length_glyph > 0) {
      uint16_t num_contours = file.read16();

      Dir2 min, max;
      min.x = static_cast<float>(std::bit_cast<int16_t>(file.read16())) * inv_units_per_em_f;
      min.y = static_cast<float>(std::bit_cast<int16_t>(file.read16())) * inv_units_per_em_f;

      max.x = static_cast<float>(std::bit_cast<int16_t>(file.read16())) * inv_units_per_em_f;
      max.y = static_cast<float>(std::bit_cast<int16_t>(file.read16())) * inv_units_per_em_f;

      /******************/
      /*  simple glyph  */
      /******************/

      if (num_contours >= 0) {
        std::vector<uint16_t> end_contours = std::vector<uint16_t>(num_contours);
        for (uint16_t i = 0; i < num_contours; i++)
          end_contours[i] = file.read16();

        uint16_t num_points = end_contours.back() + 1;
        this->glyphs[loff + 1] = ttf_glyph_data {
          .raster_information = ttf_glyph_simple_data { 
            .points = std::vector<Dir2>(num_points), 
            .flags = std::vector<uint8_t> (num_points),
            .offset = end_contours
          }, 
          .bounding_box = {min, max},
          .left_bearing = 0.f,
          .contours = num_contours
        };

        uint16_t int_len = file.read16();
        file.skip (int_len);
       
        ttf_glyph_simple_data& data_ri = std::get<ttf_glyph_simple_data>(this->glyphs[loff + 1].raster_information);

        uint32_t i = 0;
        while (i < num_points) {
          uint8_t flag = file.read8 ();
          int32_t many = flag & 0b1000 ? file.read8() + 1 : 1;
          while (many > 0 && i < num_points) {
            data_ri.flags[i++] = flag;
            many--;
          }
        }

        // x coordenates.
        data_ri.points[0].x = inv_units_per_em_f * (
          data_ri.flags[0] & 0b10 ?
            (data_ri.flags[0] & 0b10000 ? 1.f : -1.f) * static_cast<float>(file.read8 ()) : 
            (data_ri.flags[0] & 0b10000 ? 0.f: static_cast<float>(std::bit_cast<int16_t>(file.read16 ())))
        );
        for (uint32_t i = 1; i < num_points; i++) {
          uint32_t flag = data_ri.flags[i];
          float coord = (
            flag & 0b10 ?
              (flag & 0b10000 ? 1.f : -1.f) * static_cast<float>(file.read8 ()) : 
              (flag & 0b10000 ? 0.f: static_cast<float>(std::bit_cast<int16_t>(file.read16 ())))
          );
          data_ri.points[i].x = std::fmaf(coord, inv_units_per_em_f, data_ri.points[i-1].x);
        }

        // y coordenates.
        data_ri.points[0].y = (
          data_ri.flags[0] & 0b100 ?
            (data_ri.flags[0] & 0b100000 ? 1.f : -1.f) * static_cast<float>(file.read8 ()) : 
            (data_ri.flags[0] & 0b100000 ? 0.f: static_cast<float>(std::bit_cast<int16_t>(file.read16 ())))
        ) * inv_units_per_em_f;
        for (uint32_t i = 1; i < num_points; i++) {
          uint32_t flag = data_ri.flags[i];
          float coord = (
            flag & 0b100 ?
              (flag & 0b100000 ? 1.f : -1.f) * static_cast<float>(file.read8 ()) : 
              (flag & 0b100000 ? 0.f: static_cast<float>(std::bit_cast<int16_t>(file.read16 ())))
          );
          data_ri.points[i].y = std::fmaf(coord, inv_units_per_em_f, data_ri.points[i-1].y);
        }

      /********************/
      /*  compound glyph  */
      /********************/

      } else {
        /* continuará... */

        std::cout << "ahora si esta mostrando un compuesto." << std::endl;
        std::exit (-1);
      }
    }
  }


  /*****************/
  /*  readin hhea  */
  /*****************/


  file.set_file_position (tables_info[5].offset);
  if (file.read32 () != 0x00010000) {
    std::cout << "Font Error: hhea version not correct." << std::endl;
    return;
  }
  file.read32 (); file.read16 ();
  uint16_t advance_width_max = file.read16 ();
  int16_t min_left_side_bearing = std::bit_cast<int16_t>(file.read16 ());
  int16_t min_right_side_bearing = std::bit_cast<int16_t>(file.read16 ());

  min_right_side_bearing += min_right_side_bearing + min_left_side_bearing + advance_width_max;

  file.read64 ();
  if (file.read64 () != 0) {
    std::cout << "Font Error: value founded must be 0." << std::endl;
    return;
  }
  file.read16 ();

  uint16_t many_advance_widths = file.read16 ();


  /*****************/
  /*  readin hmtx  */
  /*****************/

  
  file.set_file_position (tables_info[6].offset);
  int32_t i = 1;
  this->advance_widths = std::vector<float> (many_advance_widths);
  while (i < many_advance_widths && i < static_cast<int32_t>(this->glyphs.size () - 1)) {
    this->advance_widths[i] = static_cast<float>(file.read16 ()) * inv_units_per_em_f;
    this->glyphs[i++].left_bearing = static_cast<float>(std::bit_cast<int16_t>(file.read16 ())) * inv_units_per_em_f;
  }
  for (; i < (int32_t)this->glyphs.size() - 1; i++)
    this->glyphs[i].left_bearing = static_cast<float>(std::bit_cast<int16_t>(file.read16 ())) * inv_units_per_em_f;
}
