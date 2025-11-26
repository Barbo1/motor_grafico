#include "../../../../headers/concepts/image_modifier.hpp"

ImageModifier ImageModifier::chargePNG (const std::string& path) {
  ImageModifier tex;
  int w, h;
  Uint32 * pixels = charging_PNG_to_memory (path, w, h);
  if (pixels != nullptr) {
    tex = ImageModifier (h, w, pixels);
  } else tex = ImageModifier ();
  return tex;
}
