#include "../../../../headers/concepts/gui.hpp"

GuiComponent::GuiComponent (Global* glb, uint32_t max_elems, const std::vector<SDL_Scancode>& possible_keys, int* error)
  : glb(glb),
    key_array(SDL_GetKeyboardState(nullptr)), 
    position(Dir2()),
    many_elems(0), 
    max_elems(max_elems),
    id_selected(-1), 
    last_clicking(false)
{
  *error = -1;
  if (possible_keys.size() > 128)
    return;
  for (uint32_t i = 0; i < possible_keys.size(); i++) {
    SDL_Scancode curr_char = possible_keys[i];
    for (uint32_t j = i+1; j < possible_keys.size(); j++)
      if (curr_char == possible_keys[j])
        return;
  }

  *error = 0;
  int s;

  SDL_GetKeyboardState(&s);
  this->elems = new GuiElement[this->max_elems];
  this->many_keys = s = std::min(s, static_cast<int>(possible_keys.size()));
  this->pressed_keys[0] = 0;
  this->pressed_keys[1] = 0;
  for (int32_t i = 0; i < s; i++)
    this->admited_keys[i] = possible_keys[i];
}
