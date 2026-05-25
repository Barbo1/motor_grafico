#include "../../../../headers/concepts/gui.hpp"
#include <cstdint>

void GuiComponent::test () {
  // obtain click position and test if click is pressed.
  int x, y;
  Dir2 click_position;
  bool clicking;
  uint32_t mask = SDL_GetMouseState(&x, &y);
  clicking = mask & SDL_BUTTON(SDL_BUTTON_LEFT);
  click_position = Dir2(x, y);

  // process pressed keys.
  SDL_Keymod mod = SDL_GetModState();
  int upper = ((mod & KMOD_LSHIFT) || (mod & KMOD_RSHIFT)) ^ (mod & KMOD_CAPS);
  uint64_t new_pressed_keys[2];
  new_pressed_keys[0] = 0;
  new_pressed_keys[1] = 0;
  for (uint32_t i = 0; i < this->many_keys; i++) {
    bool is_pressed = this->key_array[this->admited_keys[i]];
    uint32_t pos = i / 64;
    new_pressed_keys[pos] |= static_cast<uint64_t>(is_pressed) << (i & 63);
    if (is_pressed) {
      char curr_char = *SDL_GetScancodeName(this->admited_keys[i]);
      if (!upper && 65 <= curr_char && 90 >= curr_char)
        curr_char += 32;
    }
  }
  uint64_t xor_pressed_keys[2] = {
    (new_pressed_keys[0] ^ this->pressed_keys[0]) & new_pressed_keys[0], 
    (new_pressed_keys[1] ^ this->pressed_keys[1]) & new_pressed_keys[1]
  };
  this->pressed_keys[0] = new_pressed_keys[0];
  this->pressed_keys[1] = new_pressed_keys[1];

  int32_t selected = -1;

  for (int32_t i = 0; i < this->many_elems; i++) {
    switch (this->elems[i].type) {
      case GUITypeButton: {
        Button* button = static_cast<Button*>(this->elems[i].ptr);
        bool test = test_point_inside_square(click_position, button->position + this->position, button->dims * 0.5f);
        if (clicking && (test || (this->last_clicking && this->id_selected == i))) {
          selected = i;
          this->elems[i].state = GUIStateSelected;
        } else if (test) {
          this->elems[i].state = GUIStateObserverd;
        } else {
          this->elems[i].state = GUIStateQuiet;
        }
      }
      break;

      case GUITypeCheckBox: {
        CheckBox* check = static_cast<CheckBox*>(this->elems[i].ptr);
        bool test = test_point_inside_square(
          click_position, 
          check->position + this->position, 
          check->dims * 0.5f
        );
        if (clicking && test && !this->last_clicking) {
          selected = i;
          if (check->last_state != GUIStateSelected)
            check->active = !check->active;
          this->elems[i].state = GUIStateSelected;
        } else if (test) {
          this->elems[i].state = GUIStateObserverd;
        } else {
          this->elems[i].state = GUIStateQuiet;
        }
        check->last_state = this->elems[i].state;
      }
      break;

      case GUITypeSlider: {
        Slider* slider = static_cast<Slider*>(this->elems[i].ptr);
        Dir2 aux = this->position + slider->position;
        Dir2 v;
        float denom;
        switch (slider->direction) {
          case SliderDirectionX:
            v = Dir2(slider->base_dims.x, 0.f);
            denom = 1.f / (slider->base_dims.x * slider->base_dims.x);
            break;
          case SliderDirectionY:
            v = Dir2(0.f, slider->base_dims.y);
            denom = 1.f / (slider->base_dims.y * slider->base_dims.y);
            break;
          default:
            v = Dir2(1.f, 0.f);
            denom = 1.f;
        }
        Dir2 P = v.nmadd(0.5f, aux);

        // testing selection.
        bool test = test_point_inside_square(
          click_position, 
          aux, 
          slider->base_dims * 0.5f
        ) || test_point_inside_square(
          click_position, 
          v.madd(static_cast<float>(slider->curr_index), this->position + P),
          slider->sign_dims * 0.5f
        );

        if (clicking && (test || (this->last_clicking && this->id_selected == i))) {
          selected = i;
          this->elems[i].state = GUIStateSelected;

          // calculate new index.
          float projection_coef = (v * (click_position - P)) * denom;
          float bounded_coef = std::min(1.f, std::max(0.f, projection_coef));
          slider->curr_index = std::lround(bounded_coef * slider->max_index);
        } else if (test) {
          this->elems[i].state = GUIStateObserverd;
        } else {
          this->elems[i].state = GUIStateQuiet;
        }
      }
      break;

      case GUITypeTextBox: {
        TextBox* textbox = static_cast<TextBox*>(this->elems[i].ptr);
        bool test = test_point_inside_square(
          click_position, 
          textbox->position + this->position, 
          textbox->dims * 0.5f
        );
        
        if ((this->id_selected == i && !clicking) || (clicking && test && !this->last_clicking)) {
          selected = i;
          this->elems[i].state = GUIStateSelected;

          // response to key press.
          for (uint32_t i = 0; i < this->many_keys; i++) {
            bool is_pressed = xor_pressed_keys[i / 64] & (1ULL << (i & 63));
            uint32_t prev_text_len = textbox->text_len;
            switch (this->admited_keys[i]) {
              case SDL_SCANCODE_RETURN:
                textbox->config = (textbox->config & 0xFFFFFFFE) | is_pressed;
              case SDL_SCANCODE_RIGHT:
                if (is_pressed && textbox->curr_pos < textbox->text_len)
                  textbox->curr_pos++;
                break;
              case SDL_SCANCODE_LEFT:
                if (is_pressed && 0 < textbox->curr_pos)
                  textbox->curr_pos--;
                break;
              case SDL_SCANCODE_BACKSPACE:
                if (is_pressed && 0 < textbox->curr_pos) {
                  textbox->copial_patra();
                  textbox->curr_pos--;
                  textbox->text_len--;
                }
                break;
              case SDL_SCANCODE_SPACE:
                if (is_pressed && textbox->text_len < textbox->max_len) {
                  textbox->copial_palante();
                  textbox->text[textbox->curr_pos] = ' ';
                  textbox->text_len++;
                  textbox->curr_pos++;
                }
                break;
              default:
                if (is_pressed && textbox->text_len < textbox->max_len) {
                  textbox->copial_palante();
                  char curr_char = *SDL_GetScancodeName(this->admited_keys[i]);
                  if (!upper && 65 <= curr_char && 90 >= curr_char)
                    curr_char += 32;
                  textbox->text[textbox->curr_pos] = curr_char;
                  textbox->text_len++;
                  textbox->curr_pos++;
                }
            }

            float xdev;
            if (is_pressed && textbox->text_len != 0) {
              if (textbox->config & 4) {
                if (textbox->text_len < prev_text_len) {
                  textbox->window_end = textbox->text_len;
                  textbox->window_start = textbox->gs->get_left_window(
                    textbox->get_text(), 
                    textbox->text_len,
                    textbox->letter_size,
                    textbox->dims.x,
                    &xdev
                  );
                  textbox->xdev = xdev - textbox->dims.x;
                } else if (textbox->window_end < textbox->curr_pos) {
                  textbox->window_end = textbox->curr_pos;
                  textbox->window_start = textbox->gs->get_left_window(
                    textbox->get_text(), 
                    textbox->curr_pos,
                    textbox->letter_size,
                    textbox->dims.x,
                    &xdev
                  );
                  textbox->xdev = xdev - textbox->dims.x;
                } else if (textbox->curr_pos <= textbox->window_start) {
                  textbox->config = textbox->config & 0xFFFFFFFB;
                  textbox->window_start = textbox->curr_pos;
                  textbox->window_end = textbox->gs->get_right_window(
                    textbox->get_text(), 
                    textbox->curr_pos,
                    textbox->letter_size,
                    textbox->dims.x,
                    &xdev
                  );
                  textbox->xdev = xdev - textbox->dims.x;
                }
              } else {
                if (textbox->window_end <= textbox->curr_pos) {
                  textbox->config = (textbox->config & 0xFFFFFFFB) | 4;
                  textbox->window_end = textbox->curr_pos;
                  textbox->window_start = textbox->gs->get_left_window(
                    textbox->get_text(), 
                    textbox->curr_pos,
                    textbox->letter_size,
                    textbox->dims.x,
                    &xdev
                  );
                  textbox->xdev = xdev - textbox->dims.x;
                } else if (textbox->curr_pos <= textbox->window_start) {
                  textbox->window_start = textbox->curr_pos;
                  textbox->window_end = textbox->gs->get_right_window(
                    textbox->get_text(), 
                    textbox->curr_pos,
                    textbox->letter_size,
                    textbox->dims.x,
                    &xdev
                  );
                  textbox->xdev = xdev - textbox->dims.x;
                }
              }
            }
          }
        } else if (test) {
          this->elems[i].state = GUIStateObserverd;
        } else {
          this->elems[i].state = GUIStateQuiet;
        }
      }
      break;

      default: break;
    }
  }

  if (clicking && !this->last_clicking) {
    this->id_selected = selected;
  }
  this->last_clicking = clicking;
}
