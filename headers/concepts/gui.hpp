#include "../concepts/glyph_system.hpp"
#include "../concepts/visualizer.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <array>
#include <functional>
#include <cstdint>
#include <bitset>
#include <iostream>

enum GuiElementType {
  GUITypeButton,
  GUITypeCheckBox,
  GUITypeTextBox,
  GUITypeSlider
};

enum GuiElementState {
  GUIStateQuiet,
  GUIStateObserverd,
  GUIStateSelected
};

enum SliderDirection {
  SliderDirectionX,
  SliderDirectionY
};

struct GuiElement {
  void* ptr;
  GuiElementType type;
  GuiElementState state;
};

template<std::size_t N>
class GuiComponent;

inline bool test_point_inside_square(const Dir2& P, const Dir2& SQP, const Dir2& SQD) {
  __m128 opr = _mm_mul_ps(_mm_sub_ps(SQP.v, P.v), _mm_rcp_ps(SQD.v));
  __m128i abs = _mm_and_si128(_mm_set1_epi32(0x7FFFFFFF), _mm_castps_si128(opr));
  __m128i cmp = _mm_cmpgt_epi32(_mm_set1_epi32(0x3F800000), abs);
  return (_mm_movemask_epi8(cmp) & 0xFF) == 0xFF;
}

class Button {
  private:
    std::function<void(Dir2)> selected_fn;
    std::function<void(Dir2)> observed_fn;
    std::function<void(Dir2)> quiet_fn;
    Dir2 position, dims;

  public:
    Button (
      std::function<void(Dir2)> selected_fn,
      std::function<void(Dir2)> observed_fn,
      std::function<void(Dir2)> quiet_fn,
      Dir2 position,
      Dir2 dims
    ) noexcept;

    void set_selected_fn (std::function<void(Dir2)> fn);
    void set_observed_fn (std::function<void(Dir2)> fn);
    void set_quiet_fn (std::function<void(Dir2)> fn);

    void set_position (const Dir2& pos);
    Dir2 get_position () const;

    void set_dimentions (const Dir2& dimentions);
    Dir2 get_dimentions () const;

    template<std::size_t N> friend class GuiComponent;
};

class CheckBox {
  private:
    std::function<void(Dir2)> active_fn;
    std::function<void(Dir2)> deactive_fn;
    Dir2 position, dims;
    GuiElementState last_state = GuiElementState::GUIStateQuiet;
    bool active;

  public:
    CheckBox (
      std::function<void(Dir2)> active_fn,
      std::function<void(Dir2)> deactive_fn,
      Dir2 position,
      Dir2 dims,
      bool active
    ) noexcept;

    void set_active_fn (std::function<void(Dir2)> fn);
    void set_deactive_fn (std::function<void(Dir2)> fn);

    void set_position (const Dir2& pos);
    Dir2 get_position () const;

    void set_dimentions (const Dir2& dimentions);
    Dir2 get_dimentions () const;

    template<std::size_t N> friend class GuiComponent;
};

class Slider {
  private:
    std::function<void(Dir2)> base_fn;
    std::function<void(Dir2)> sign_fn;
    Dir2 position, base_dims, sign_dims;
    float max_index;
    uint32_t curr_index;
    SliderDirection direction;

  public:
    Slider (
      std::function<void(Dir2)> base_fn,
      std::function<void(Dir2)> sign_fn,
      Dir2 position,
      Dir2 base_dims,
      Dir2 sign_dims,
      uint32_t number_values,
      SliderDirection direction
    ) noexcept;

    void set_base_fn (std::function<void(Dir2)> fn);
    void set_sign_fn (std::function<void(Dir2)> fn);

    void set_position (const Dir2& pos);
    Dir2 get_position () const;

    void set_base_dimentions (const Dir2& dimentions);
    Dir2 get_base_dimentions () const;
    void set_sign_dimentions (const Dir2& dimentions);
    Dir2 get_sign_dimentions () const;

    uint32_t get_current_value () const;

    template<std::size_t N> friend class GuiComponent;
};

class TextBox {
  private:
    GlyphsSystem* gs;
    std::function<void(Dir2)> background;
    Visualizer<D2FIG> cursor_image;
    char* text;
    Dir2 position, dims;
    uint32_t 
      letter_size, 
      text_len, 
      max_len, 
      curr_pos, 
      cursor_dev, 
      config;
    SDL_Color letter_color;

  public:
    TextBox (
      Global* glb,
      GlyphsSystem* gs,
      std::function<void(Dir2)> background,
      Dir2 position, 
      Dir2 dims,
      uint32_t max_len,
      SDL_Color letter_color,
      uint32_t letter_size,
      bool various_lines
    ) noexcept;

    ~TextBox();

    void set_background_fn (std::function<void(Dir2)> fn);

    Dir2 get_position() const;
    void set_position(const Dir2&);

    void set_dimentions (const Dir2&);
    Dir2 get_dimentions () const;
    
    std::string get_text() const;
    bool set_text(const std::string&);
    bool set_cursor(uint32_t pos); // return true if the reposition was made.
    uint32_t get_cursor();

    template<std::size_t N> friend class GuiComponent;
};


template<std::size_t N>
class GuiComponent {
  private:
    Global* glb;
    const Uint8* key_array;

    // general data.
    std::array<GuiElement, N> elems;
    Dir2 position;
    int32_t many_elems;

    // generating click mutex.
    int32_t id_selected;
    bool last_clicking;

    // text input.
    SDL_Scancode admited_keys[128];
    uint64_t pressed_keys[2];
    uint32_t many_keys;

  public:
    GuiComponent (Global* glb, const std::vector<SDL_Scancode>& possible_keys, int* error)
      : glb(glb),
        key_array(SDL_GetKeyboardState(nullptr)), 
        position(Dir2()),
        many_elems(0), 
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
      this->many_keys = s = std::min(s, static_cast<int>(possible_keys.size()));
      this->pressed_keys[0] = 0;
      this->pressed_keys[1] = 0;
      for (int32_t i = 0; i < s; i++)
        this->admited_keys[i] = possible_keys[i];
    }

    void add (Button* button) {
      this->elems[this->many_elems] = GuiElement {
        .ptr = button, 
        .type = GUITypeButton, 
        .state = GUIStateQuiet
      };
      this->many_elems++;
    }

    void add (CheckBox* checkbox) {
      this->elems[this->many_elems] = GuiElement {
        .ptr = checkbox, 
        .type = GUITypeCheckBox, 
        .state = GUIStateQuiet
      };
      this->many_elems++;
    }

    void add (Slider* slider) {
      this->elems[this->many_elems] = GuiElement {
        .ptr = slider, 
        .type = GUITypeSlider, 
        .state = GUIStateQuiet
      };
      this->many_elems++;
    }

    void add (TextBox* txtb) {
      this->elems[this->many_elems] = GuiElement {
        .ptr = txtb, 
        .type = GUITypeTextBox, 
        .state = GUIStateQuiet
      };
      this->many_elems++;
    }

    // iterating elements
    void test () {
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
                if (is_pressed) {
                  switch (this->admited_keys[i]) {
                    case SDL_SCANCODE_RIGHT:
                      if (textbox->curr_pos < textbox->text_len)
                        textbox->curr_pos++;
                      break;
                    case SDL_SCANCODE_LEFT:
                      if (0 < textbox->curr_pos)
                        textbox->curr_pos--;
                      break;
                    case SDL_SCANCODE_BACKSPACE:
                      if (0 < textbox->text_len && 0 < textbox->curr_pos) {
                        textbox->curr_pos--;
                        textbox->text_len--;
                      }
                      break;
                    case SDL_SCANCODE_SPACE:
                      if (textbox->text_len < textbox->max_len) {
                        textbox->text[textbox->curr_pos] = ' ';
                        if (textbox->text_len == textbox->curr_pos)
                          textbox->text_len++;
                        textbox->curr_pos++;
                      }
                      break;
                    default:
                      if (textbox->text_len < textbox->max_len) {
                        char curr_char = *SDL_GetScancodeName(this->admited_keys[i]);
                        if (!upper && 65 <= curr_char && 90 >= curr_char)
                          curr_char += 32;
                        textbox->text[textbox->curr_pos] = curr_char;
                        if (textbox->text_len == textbox->curr_pos)
                          textbox->text_len++;
                        textbox->curr_pos++;
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

    // printing.
    void print () {
      for (int32_t i = 0; i < this->many_elems; i++) {
        switch (this->elems[i].type) {
          case GUITypeButton: {
            Button* button = static_cast<Button*>(this->elems[i].ptr);
            switch (this->elems[i].state) {
              case GUIStateSelected: 
                button->selected_fn(this->position + button->position); 
                break;
              case GUIStateObserverd: 
                button->observed_fn(this->position + button->position); 
                break;
              case GUIStateQuiet: 
                button->quiet_fn(this->position + button->position); 
                break;
            }
          } 
          break;
          case GUITypeCheckBox: {
            CheckBox* check = static_cast<CheckBox*>(this->elems[i].ptr);
            if (check->active)
              check->active_fn(this->position + check->position); 
            else
              check->deactive_fn(this->position + check->position); 
          }
          break;
          case GUITypeSlider: {
            Slider* slider = static_cast<Slider*>(this->elems[i].ptr);
            Dir2 aux = this->position + slider->position;
            slider->base_fn(aux);

            Dir2 delta;
            switch (slider->direction) {
              case SliderDirectionX:
                delta = Dir2(slider->base_dims.x , 0.f);
                break;
              case SliderDirectionY:
                delta = Dir2(0.f, slider->base_dims.y);
                break;
              default:
                delta = Dir2(1.f, 0.f);
            }
            float distance = static_cast<float>(slider->curr_index) / slider->max_index;
            slider->sign_fn(delta.madd(distance - 0.5f, aux));
          }
          break;
          case GUITypeTextBox: {
            TextBox* textbox = static_cast<TextBox*>(this->elems[i].ptr);
            Dir2 aux = this->position + textbox->position;
            textbox->background(aux);

            Dir2 v = Dir2 (-textbox->dims.x, 0.f);
            Dir2 P = v.madd(0.5f, aux);
            std::string str = textbox->get_text();
            textbox->gs->print(
              str, 
              textbox->letter_size, 
              textbox->letter_color, 
              P
            );

            uint32_t total_length = textbox->gs->get_length(str, textbox->curr_pos, textbox->letter_size);
            textbox->cursor_image.draw(this->glb, P + Dir2(total_length + 2 * textbox->cursor_dev, 0.f));
          }
          break;
          default: break;
        }
      }
    }

    // methods for elements.
    GuiElementType get_type(uint32_t pos) {
      return this->elems[pos].type;
    }

    GuiElementState get_state(uint32_t pos) {
      return this->elems[pos].state;
    }

    void set_position(const Dir2& pos) {
      this->position = pos;
    }

    bool any_selected () {
      return this->id_selected > -1;
    }

    Dir2 get_position() {
      return this->position;
    }
};
