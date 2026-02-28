#include "../../headers/primitives/global.hpp"
#include "../../headers/primitives/rasterizer.hpp"

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
    
struct ttf_glyph_compound_data { 
  std::vector<uint16_t> components; 
  std::vector<uint16_t> flags; 
  std::vector<std::pair<uint16_t[2], float[4]>> tranformation; 
};

struct ttf_glyph_simple_data { 
  std::vector<Dir2> points; 
  std::vector<uint8_t> flags; 
  std::vector<uint16_t> offset; 
};

struct ttf_glyph_data {
  std::variant <ttf_glyph_simple_data, ttf_glyph_compound_data> raster_information;
  std::pair<Dir2, Dir2> bounding_box;
  float left_bearing;
  uint32_t many;
};

class GlyphsSystem {
  private:
    static constexpr std::array<uint32_t, 9> required_tags = {
      0x636D6170, 0x676C7966, 0x68656164, 0x68686561, 0x686D7478, 
      0x6C6F6361, 0x6D617870, 0x6E616D65, 0x706F7374
    };

    static constexpr std::array<uint32_t, 9> tags_order = {
      0x68656164, // head
      0x6D617870, // maxp
      0x636D6170, // cmap
      0x6C6F6361, // loca
      0x676C7966, // glyf
      0x68686561, // hhea
      0x686D7478, // hmtx
      0x706F7374, // post
      0x6E616D65  // name
    };

    struct TTFCachedGlyphInfo {
      SDL_Texture* tex;
      uint32_t w;
      uint32_t h;
    };

    Global* glb;

    // already cached glyphs.
    std::map<uint32_t, TTFCachedGlyphInfo> cached_glyphs;
    std::vector<float> advance_widths;

    // glyphs metadata.
    std::map<char16_t, uint16_t> mapping;
    std::vector<ttf_glyph_data> glyphs;
    uint32_t max_points, max_component_depth;
    float inv_units_per_em_f;

    // state of the metadata.
    bool is_meta;

    SDL_Surface* raster (char16_t, uint32_t);
    static uint32_t get_key(char16_t, uint32_t);
    
  public:
    /* the contructor receives the Global state and the path to the ttf file. 
     * The returning parameter 'error' could be:
     *  + -1 or -2 for sequential file reader errors,
     *  + -3 for present information missmatching whats expected,
     *  + -4 for information not present in the file,
     *  + -5 inconsistencies between inner information,.
     * */
    GlyphsSystem (Global* glb, std::string path, int* error);
    ~GlyphsSystem () = default;

    /* functions meant to print a letter or string, in the screen, on a specified 
     * position. 
     * */
    void print (char16_t, uint32_t, Dir2);
    void print (std::u16string, uint32_t, Dir2);

    /* completely clears the cached glyphs(already rasterized characters) from 
     * memory. 
     * */
    void clear_cache ();

    /* erase a specified character of a specified size from the cached glyphs. */
    void clear_glyph (char16_t, uint32_t);

    /* clears the metadata of the characters, leaving more space but making 
     * impossible to raster new ones. 
     * */
    void clear_meta ();
};
