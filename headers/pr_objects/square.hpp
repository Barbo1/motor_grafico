#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/visualizer.hpp"
#include "../primitives/global.hpp"
#include <cstdint>

class Circle;
class Line;

class Square: public Physical {
  private:
    float width;
    float height;
    Visualizer<D2FIG> texture;
    Global* glb;

  public: 
    Square () noexcept;
    Square (
      Global* glb, uint32_t height, uint32_t width, AngDir2 center, 
      float density = 0, float f_k = 0, bool movible = true, bool colidable = true, 
      SDL_Color* color = nullptr
    ) noexcept;
    Square (const Square &) noexcept;
    Square (Square &&) noexcept;
    Square & operator= (const Square &) noexcept;
    Square & operator= (Square &&) noexcept;

    void set_texture (Visualizer<D2FIG>);
    void set_height (float);
    void set_width (float);

    float get_height () const;
    float get_width () const;
    Visualizer<D2FIG> get_texture () const;

    void draw ();

    friend Physical;

    friend bool test_collition (Physical &, Physical &);
    friend bool test_collition (Square&, Square&);
    friend bool test_collition (Circle&, Square&);
    friend bool test_collition (Square&, Circle&);
    friend bool test_collition (Square&, Line&);

    friend void resolve_collition (Physical &, Physical &);
    friend void resolve_collition (Square&, Square&);
    friend void resolve_collition (Square&, Circle&);
    friend void resolve_collition (Circle&, Square&);
    friend void resolve_collition (Square&, Line&);

    friend void correct_collition (Physical&, Physical&);
    friend void correct_collition (Square&, Square&);
    friend void correct_collition (Square&, Circle&);
    friend void correct_collition (Circle&, Square&);
    friend void correct_collition (Square&, Line&);
};
