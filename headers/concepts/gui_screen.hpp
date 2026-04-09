#include "../concepts/visualizer.hpp"
#include "../concepts/glyph_system.hpp"

#include <array>
#include <cstdint>
#include <string_view>

class GuiElementModel {
  protected:
    Dir2 position;

  public:
    void set_position(const Dir2& pos) {
      this->position = pos;
    }

    Dir2 get_position() const {
      return this->position;
    }

    virtual bool test_observation();
};

class Button final: private GuiElementModel {
  private:
    Visualizer<D2FIG> selected_img;
    Visualizer<D2FIG> observed_img;
    Visualizer<D2FIG> quiet_img;

    Dir2 dims;

  public:
    Button (
      const Dir2& pos, const Dir2& dims, 
      Visualizer<D2FIG> quiet_img,
      Visualizer<D2FIG> observed_img,
      Visualizer<D2FIG> selected_img
    ) noexcept;

    bool test_observation() override;
};

class CheckBox final: private GuiElementModel {
  private:
    Visualizer<D2FIG> active_img;
    Visualizer<D2FIG> deactive_img;

    Dir2 dims;
    bool state;

  public:
    CheckBox (
      const Dir2& pos, const Dir2 dims,
      Visualizer<D2FIG> active_img,
      Visualizer<D2FIG> deactive_img
    );

    bool test_observation() override;
};

class TextBox final: private GuiElementModel {
  private:
    Visualizer<D2FIG> background;

    std::string text;
    Dir2 dims;
    SDL_Color gs_color;
    uint32_t gs_size;

  public:
    TextBox (
      const Dir2& pos, const Dir2& dims,
      SDL_Color gs_color, uint32_t gs_size,
      Visualizer<D2FIG> background 
    );

    void set_text(std::string_view);
    std::string get_text();

};

class Slider final: private GuiElementModel {
  private:
    Visualizer<D2FIG> base_img;
    Visualizer<D2FIG> sign_img;

    Dir2 base_dims;
    Dir2 sign_dims;
    uint32_t index, max_index;
    bool direction;

  public:
    Slider (
      const Dir2& bdims, const Dir2& sdims, 
      const Dir2& pos, const Dir2& max_index,
      uint32_t direction,
      Visualizer<D2FIG> bimg, 
      Visualizer<D2FIG> simg
    );
  
    bool test_observation() override;
};

enum GuiElementType {
  GUIButtonType,
  GUICheckBoxType,
  GUITextBoxType,
  GUISliderType
};

using GuiElement = std::variant<Button, CheckBox>;

template<std::size_t N>
class GuiScreen {
  private:
    std::array<GuiElement, N> elems;
    std::size_t many;

    const Global* glb;
    const GlyphsSystem* gs;
    
    uint32_t observed;
    bool selected;

  public:
    GuiScreen (const Global* glb, const GlyphsSystem* gs) {
      this->glb = glb;
      this->gs = gs;
      this->many = 0;
      this->observed = 0;
      this->selected = false;
    }

    void add (const Button& button) {
      this->elems[many++] = button;
    }

    void add (const CheckBox& checkbox) {
      this->elems[many++] = checkbox;
    }

    // iterating elements
    uint32_t iterate() {
      for (uint32_t i = 0; i < this->many; i++) {
        /* probablemente usar std::visit. */
      }
    }

    // methods for elements.
    GuiElementType get_type(uint32_t);
    void set_position(const Dir2&);
    Dir2 get_position();
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
