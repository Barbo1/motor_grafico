#include "../../../../headers/concepts/gui.hpp"
#include <cmath>

Label::Label (
  Global* glb,
  GlyphsSystem* gs,
  Arena& arena,
  std::function<void(Dir2)> background,
  Dir2 position, 
  Dir2 dims,
  uint32_t max_len,
  SDL_Color letter_color,
  uint32_t letter_size,
  int *error
) noexcept:
   background(background),
   gs(gs),
   position(position),
   dims(dims),
   letter_color(letter_color),
   letter_size(letter_size),
   max_len(max_len),
   text_len(0),
   config(0)
{
  this->text = arena.talloc<char>(max_len);
  if (this->text == nullptr) {
    if (error != nullptr)
      *error = -1;
    return;
  }
  this->text_area = SDL_CreateTexture(
    glb->get_render(), 
    SDL_PIXELFORMAT_RGBA8888, 
    SDL_TEXTUREACCESS_TARGET, 
    dims.x(), 
    dims.y()
  );
}
