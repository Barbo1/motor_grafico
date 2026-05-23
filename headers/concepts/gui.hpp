#include "../concepts/glyph_system.hpp"
#include "../concepts/visualizer.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <functional>
#include <cstdint>

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

    friend class GuiComponent;
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

    bool get_active () const;

    friend class GuiComponent;
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

    friend class GuiComponent;
};

class TextBox {
  private:
    GlyphsSystem* gs;
    std::function<void(Dir2)> background;
    Visualizer<D2FIG> cursor_image;
    SDL_Texture* text_area;
    char* text;
    Dir2 position, dims;
    SDL_Color letter_color;
    uint32_t 
      letter_size, 
      text_len, 
      max_len, 
      curr_pos, 
      cursor_dev, 
      config,
      window_start,
      window_end;

    /* position ->   0       1              2
     * config   -> ENTER PRINT_POS WINDOW_AGAINST_RIGHT
     * 
     * ENTER is set if the enter key is pressed,
     * PRINT_POS is set if the text is set over the end or the start of the textbox.
     * */

    void copial_patra() {
      for (uint32_t i = this->curr_pos-1; i < this->text_len; i++) {
        this->text[i] = this->text[i+1];
      }
    }

    void copial_palante() {
      for (uint32_t i = this->text_len; this->curr_pos < i; i--) {
        this->text[i] = this->text[i-1];
      }
    }

  public:
    TextBox (
      Global* glb,
      GlyphsSystem* gs,
      std::function<void(Dir2)> background,
      Dir2 position, 
      Dir2 dims,
      uint32_t max_len,
      SDL_Color letter_color,
      uint32_t letter_size
    ) noexcept;

    ~TextBox();

    void set_background_fn (std::function<void(Dir2)> fn);

    Dir2 get_position() const;
    void set_position(const Dir2&);

    void set_dimentions (const Dir2&);
    Dir2 get_dimentions () const;
    
    std::string get_text() const;
    bool set_text(const std::string&);

    bool set_cursor(uint32_t pos);
    uint32_t get_cursor() const;

    bool get_active () const;

    friend class GuiComponent;
};


class GuiComponent {
  private:
    Global* glb;
    const Uint8* key_array;

    // general data.
    GuiElement* elems;
    Dir2 position;
    int32_t many_elems, max_elems;

    // generating click mutex.
    int32_t id_selected;
    bool last_clicking;

    // text input.
    SDL_Scancode admited_keys[128];
    uint64_t pressed_keys[2];
    uint32_t many_keys;

  public:
    GuiComponent (
      Global* glb, 
      uint32_t max_elems, 
      const std::vector<SDL_Scancode>& possible_keys, 
      int* error
    );

    void add (Button* button);
    void add (CheckBox* checkbox);
    void add (Slider* slider);
    void add (TextBox* txtb);

    void test ();
    void print ();

    // methods for elements.
    GuiElementType get_type(uint32_t pos) const;
    GuiElementState get_state(uint32_t pos) const;
    void set_position(const Dir2& pos);
    bool any_selected () const;
    Dir2 get_position() const;
};
