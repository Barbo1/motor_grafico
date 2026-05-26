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
  uint32_t letter_size
) noexcept:
   background(background),
   gs(gs),
   text(new char16_t[max_len]),
   position(position),
   dims(dims),
   letter_color(letter_color),
   letter_size(letter_size),
   text_len(0),
   max_len(max_len),
   curr_pos(0),
   config(0),
   window_start(0),
   window_end(1),
   xdev(0.f)
{
  this->text_area = SDL_CreateTexture(
    glb->get_render(), 
    SDL_PIXELFORMAT_RGBA8888, 
    SDL_TEXTUREACCESS_TARGET, 
    this->dims.x + gs->get_max_advance(letter_size), 
    dims.y
  );
  float cursor_dev = log2(letter_size) * 0.63092975f;
  this->cursor_dev = cursor_dev;
  this->cursor_image = ImageModifier::square(
    gs->get_ascent(letter_size) - gs->get_descent(letter_size),
    cursor_dev,
    letter_color
  ).cast(glb);

  SDL_Texture* actual_target = SDL_GetRenderTarget(glb->get_render());
  SDL_SetRenderTarget(glb->get_render(), this->text_area);
    this->cursor_image.draw(
      glb, 
      Dir2(this->cursor_dev , this->dims.y * 0.5f)
    );
  SDL_SetRenderTarget(glb->get_render(), actual_target);

}
