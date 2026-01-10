#include "../../headers/primitives/global.hpp"

#include <SDL2/SDL.h>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <array>
#include <variant>
#include <cstring>


struct ttf_table_info {
  uint32_t tag;
  uint32_t checksum;
  uint32_t offset;
  uint32_t length;
};
    
struct ttf_glyph_compound_data { std::vector<char16_t> components; std::vector<float[6]> tranformation; };
struct ttf_glyph_simple_data { std::vector<Dir2> points; std::vector<uint8_t> flags; std::vector<uint16_t> offset; };
struct ttf_glyph_data {
  std::variant <ttf_glyph_simple_data, ttf_glyph_compound_data> raster_information;
  std::pair<Dir2, Dir2> bounding_box;
  float left_bearing;
  uint32_t contours;
};

class GlyphsSystem {
  private:
    static constexpr std::array<uint32_t, 9> required_tags = {
      0x636D6170, 0x676C7966, 0x68656164, 0x68686561, 0x686D7478, 
      0x6C6F6361, 0x6D617870, 0x6E616D65, 0x706F7374
    };

    /* tables order:
     *  - head
     *  - maxp
     *  - cmap
     *  - loca
     *  - glyf
     *  - hhea
     *  - hmtx
     *  - post
     *  - name
     * */
    static constexpr std::array<uint32_t, 9> tags_order = {
      0x68656164, 0x6D617870, 0x636D6170, 0x6C6F6361, 0x676C7966, 
      0x68686561, 0x686D7478, 0x706F7374, 0x6E616D65
    };

    struct TTFCachedGlyphInfo {
      SDL_Texture* tex;
      uint32_t w;
      uint32_t h;
    };

    Global* glb;
    std::map<char16_t, uint16_t> mapping;
    std::map<uint32_t, TTFCachedGlyphInfo> cached_glyphs;
    std::vector<ttf_glyph_data> glyphs;
    std::vector<float> advance_widths;
    
  public:
    GlyphsSystem (Global* glb, std::string path, int* error);
    ~GlyphsSystem () = default;

    void print (char16_t, uint32_t, Dir2);
    void print (std::u16string, uint32_t, Dir2);

    void clear_cache ();
    void clear_glyph (char16_t, uint32_t);
    void clear_sequence (uint32_t);

    SDL_Surface* raster (char16_t, uint32_t);
};
