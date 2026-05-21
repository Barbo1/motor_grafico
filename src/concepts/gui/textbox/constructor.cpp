#include "../../../../headers/concepts/gui.hpp"
#include "../../../../headers/concepts/image_modifier.hpp"
#include <cmath>

/*
    GlyphsSystem* gs;
    std::function<void(Dir2)> background;
    Visualizer<D2FIG> cursor_image;
    char* text;
    Dir2 position, dims;
    SDL_Color letter_color;
    uint32_t 
      letter_size, 
      text_len, 
      max_len, 
      curr_pos, 
      cursor_dev, 
      config;
 * */

TextBox::TextBox (
  Global* glb,
  GlyphsSystem* gs,
  std::function<void(Dir2)> background,
  Dir2 position, 
  Dir2 dims,
  uint32_t max_len,
  SDL_Color letter_color,
  uint32_t letter_size
) noexcept: gs(gs),
   background(background),
   text(new char[max_len]),
   position(position),
   letter_color(letter_color),
   letter_size(letter_size),
   text_len(0),
   max_len(max_len),
   curr_pos(0),
   config(0)
{
  this->dims = dims;
  this->dims.x = std::max(this->dims.x, gs->get_max_advance(letter_size) * max_len) * 0.5f;
  this->cursor_dev = log2(letter_size) * 0.63092975f;
  this->cursor_image = ImageModifier::square(
    gs->get_ascent(letter_size) - gs->get_descent(letter_size),
    this->cursor_dev,
    letter_color
  ).cast(glb);
}
