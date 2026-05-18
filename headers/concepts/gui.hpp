#include "../concepts/glyph_system.hpp"

#include <array>
#include <functional>
#include <cstdint>
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
    Dir2 position;
    Dir2 dims;

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
    Dir2 position;
    Dir2 dims;
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

struct TextBox {
  std::function<void(Dir2)> background;

  std::string text;
  Dir2 dims;
  Dir2 position;
  SDL_Color gs_color;
  uint32_t gs_size;
};


template<std::size_t N>
class GuiComponent {
  private:
    const Global* glb;
    const GlyphsSystem* gs;

    // general data.
    std::array<GuiElement, N> elems;
    Dir2 position;
    int32_t many;

    // generating mutex.
    int32_t id_selected;
    bool last_clicking;

  public:
    GuiComponent (const Global* glb, const GlyphsSystem* gs) {
      this->glb = glb;
      this->gs = gs;
      this->many = 0;
      this->last_clicking = false;
      this->id_selected = -1;
    }

    void add (Button* button) {
      this->elems[this->many] = GuiElement {
        .ptr = button, 
        .type = GUITypeButton, 
        .state = GUIStateQuiet
      };
      this->many++;
    }

    void add (CheckBox* checkbox) {
      this->elems[this->many] = GuiElement {
        .ptr = checkbox, 
        .type = GUITypeCheckBox, 
        .state = GUIStateQuiet
      };
      this->many++;
    }

    void add (Slider* slider) {
      this->elems[this->many] = GuiElement {
        .ptr = slider, 
        .type = GUITypeSlider, 
        .state = GUIStateQuiet
      };
      this->many++;
    }

    // iterating elements
    void test (Dir2 click_position, bool clicking) {
      int32_t selected = -1;

      for (int32_t i = 0; i < this->many; i++) {
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
            bool test = test_point_inside_square(click_position, check->position + this->position, check->dims * 0.5f);
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
      for (int32_t i = 0; i < this->many; i++) {
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

    Dir2 get_position() {
      return this->position;
    }
};
