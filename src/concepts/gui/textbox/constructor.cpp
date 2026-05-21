#include "../../../../headers/concepts/gui.hpp"
#include "../../../../headers/concepts/image_modifier.hpp"
#include <cmath>
    
TextBox::TextBox (
  Global* glb,
  GlyphsSystem* gs,
  std::function<void(Dir2)> background,
  Dir2 position, 
  Dir2 dims,
  uint32_t max_len,
  SDL_Color letter_color,
  uint32_t letter_size,
  bool various_lines
) noexcept: gs(gs),
   background(background),
   text(new char[max_len]),
   position(position),
   dims(dims),
   letter_size(letter_size),
   text_len(0),
   max_len(max_len),
   curr_pos(0),
   config(various_lines),
   letter_color(letter_color) 
{
  this->cursor_dev = log2(letter_size) * 0.63092975f;
  this->cursor_image = ImageModifier::square(
    gs->get_ascent(letter_size) - gs->get_descent(letter_size),
    this->cursor_dev,
    letter_color
  ).cast(glb);
}
