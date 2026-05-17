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

struct GuiElement {
  void* ptr;
  GuiElementType type;
  GuiElementState state;
};

inline bool test_point_inside_square(const Dir2& P, const Dir2& SQP, const Dir2& SQD) {
  __m128 opr = _mm_mul_ps(_mm_sub_ps(SQP.v, P.v), _mm_rcp_ps(SQD.v));
  __m128i abs = _mm_and_si128(_mm_set1_epi32(0x7FFFFFFF), _mm_castps_si128(opr));
  __m128i cmp = _mm_cmpgt_epi32(_mm_set1_epi32(0x3F800000), abs);
  return (_mm_movemask_epi8(cmp) & 0xFF) == 0xFF;
}

struct Button {
  std::function<void(Dir2)> selected_fn;
  std::function<void(Dir2)> observed_fn;
  std::function<void(Dir2)> quiet_fn;

  Dir2 position;
  Dir2 dims;
};

struct CheckBox {
  std::function<void(Dir2)> active_fn;
  std::function<void(Dir2)> deactive_fn;

  Dir2 dims;
  Dir2 position;
  bool active;
  GuiElementState last_state;
};

struct TextBox {
  std::function<void(Dir2)> background;

  std::string text;
  Dir2 dims;
  Dir2 position;
  SDL_Color gs_color;
  uint32_t gs_size;
};

struct Slider {
  std::function<void(Dir2)> base_fn;
  std::function<void(Dir2)> sign_fn;

  Dir2 position, base_dims, sign_dims;
  uint32_t curr_index, max_index;
  bool direction;
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
    void test_selected (Dir2 click_position, bool clicking) {
      int32_t selected = -1;

      for (int32_t i = 0; i < this->many; i++) {
        switch (this->elems[i].type) {
          case GUITypeButton: {
            Button* button = static_cast<Button*>(this->elems[i].ptr);
            bool test = test_point_inside_square(click_position, button->position + this->position, button->dims);
            if (clicking && test && !this->last_clicking) {
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
            bool test = test_point_inside_square(click_position, check->position + this->position, check->dims);
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
            Dir2 v = Dir2(slider->base_dims.x, 0.f);
            Dir2 aux = this->position + slider->position;
            Dir2 P = aux - v * 0.5f;

            // testing selection.
            bool test = test_point_inside_square(
              click_position, 
              slider->position + this->position, 
              slider->base_dims * 0.5f
            ) || test_point_inside_square(
              click_position, 
              this->position + P + v * static_cast<float>(slider->curr_index),
              slider->sign_dims * 0.5f
            );

            // obtain projection.
            float projection_coef = (v * (click_position - P)) / (slider->base_dims.x * slider->base_dims.x);
            float bounded_coef = std::min(1.f, std::max(0.f, projection_coef));
            int32_t new_index = std::lround(bounded_coef * static_cast<float>(slider->max_index));

            if (clicking && (test || (this->last_clicking && this->id_selected == i))) {
              selected = i;
              slider->curr_index = new_index;
              this->elems[i].state = GUIStateSelected;
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
            slider->base_fn(this->position + slider->position);

            Dir2 delta = Dir2(slider->base_dims.x , 0.f);
            Dir2 sign_pos = this->position + slider->position - delta * 0.5f;
            float distance = 
              static_cast<float>(slider->curr_index) / static_cast<float>(slider->max_index);

            slider->sign_fn(sign_pos + delta * distance);
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

/*
  + el 'void begin()' tiene que testear si alguno de los elementos esta siendo 
  observado y, en caso de que lo este, testear si esta siendo seleccionado.

  + tiene que haber un enumerado GuiElementState en el que se consideren:
    1- los elementos recien seleccionados(GES_JustPressed),
    2- los elementos previamente seleccionados(GES_PrevPressed).
  esta diferencia es necesaria para contemplar el caso de elementos que son
  seleccionados pueden ser utilizados mientras lo estan, como los Sliders,
  que tienen que ser movidos mientras este precionado el click.
  
  + en base al punto anterior, es necesario mantener una referencia hacia
  el elemento que esta seleccionado, para verificar que se siga cumpliento
  la seleccion, y si no, que se elimine la referencia para poder dejar que
  otro elemento sea seleccionado.

  + es necesario guardar un arreglo de estados, en el que se dicte las imagenes
  que van a ser utilizadas para mostrar los componentes en pantalla. Esto hace
  que se puedan separar la tarea de mostrar la interfaz y la de realizar la 
  logica necesaria para que funcione.

  + los scrollpane deben ser un objeto aparte para que pueda ser todo correctamente
  utilizado.
*/
