#include "../concepts/glyph_system.hpp"

#include <array>
#include <functional>
#include <cstdint>
#include <iostream>

enum GuiElementType {
  GUIButtonType,
  GUICheckBoxType,
  GUITextBoxType,
  GUISliderType
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
  uint32_t index, max_index;
  bool direction;
};

template<std::size_t N>
class GuiComponent {
  private:
    std::array<GuiElement, N> elems;
    std::size_t many;
    Dir2 position;

    const Global* glb;
    const GlyphsSystem* gs;

  public:
    GuiComponent (const Global* glb, const GlyphsSystem* gs) {
      this->glb = glb;
      this->gs = gs;
      this->many = 0;
    }

    void add (Button* button) {
      this->elems[many++] = {
        .ptr = button, 
        .type = GUIButtonType, 
        .state = GUIStateQuiet
      };
    }

    void add (CheckBox* checkbox) {
      this->elems[many++] = {
        .ptr = checkbox, 
        .type = GUICheckBoxType, 
        .state = GUIStateQuiet
      };
    }

    void add (Slider* slider) {
      this->elems[many++] = {
        .ptr = slider, 
        .type = GUISliderType, 
        .state = GUIStateQuiet
      };
    }

    // iterating elements
    void test_selected (Dir2 click_position, bool clicking) {
      for (uint32_t i = 0; i < this->many; i++) {
        switch (this->elems[i].type) {
          case GUIButtonType: {
            Button* button = static_cast<Button*>(this->elems[i].ptr);
            bool test = test_point_inside_square(click_position, button->position + this->position, button->dims);
            if (clicking && test) {
              this->elems[i].state = GUIStateSelected;
            } else if (test) {
              this->elems[i].state = GUIStateObserverd;
            } else {
              this->elems[i].state = GUIStateQuiet;
            }
          }
          break;
          case GUICheckBoxType: {
            CheckBox* check = static_cast<CheckBox*>(this->elems[i].ptr);
            bool test = test_point_inside_square(click_position, check->position + this->position, check->dims);
            if (clicking && test) {
              if (check->last_state != GUIStateSelected)
                check->active = !check->active;
              this->elems[i].state = GUIStateSelected;
            } else if (test) {
              this->elems[i].state = GUIStateObserverd;
            } else {
              this->elems[i].state = GUIStateQuiet;
            }
            check->last_state = this->elems[i].state;
            std::cout << this->elems[i].state << std::endl; // problema.
          }
          break;
          default: break;
        }
      }
    }

    // printing.
    void print () {
      for (uint32_t i = 0; i < this->many; i++) {
        switch (this->elems[i].type) {
          case GUIButtonType: {
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
          case GUICheckBoxType: {
            CheckBox* check = static_cast<CheckBox*>(this->elems[i].ptr);
            if (check->active)
              check->active_fn(this->position + check->position); 
            else
              check->deactive_fn(this->position + check->position); 
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
