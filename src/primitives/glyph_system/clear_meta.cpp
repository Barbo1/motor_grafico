#include "../../../headers/primitives/glyph_system.hpp"

void GlyphsSystem::clear_meta () {
  if (this->is_meta) {
    this->mapping.clear();
    this->glyphs.clear();
    this->is_meta = false;
  }
}
