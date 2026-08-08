#include "../../../headers/concepts/glyph_system.hpp"

void GlyphsSystem::clear_meta () {
  if (this->is_meta) {
    this->glyphs.clear();
    this->is_meta = false;
  }
}
