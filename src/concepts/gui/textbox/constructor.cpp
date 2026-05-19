#include "../../../../headers/concepts/gui.hpp"
    
TextBox::TextBox (
  std::function<void(Dir2)> background,
  Dir2 position, 
  Dir2 dims,
  uint32_t max_len,
  SDL_Color letter_color,
  uint32_t letter_size,
  bool various_lines
) noexcept: background(background),
   text(new char[max_len]),
   position(position),
   dims(dims),
   letter_size(letter_size),
   text_len(0),
   max_len(max_len),
   curr_pos(0),
   config(various_lines),
   letter_color(letter_color) {}
