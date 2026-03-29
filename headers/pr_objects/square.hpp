#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/visualizer.hpp"
#include "../primitives/global.hpp"
#include <cstdint>

class Circle;
class Line;
class Particle;
template<std::size_t N> class NEdge;

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

    friend bool test_collition (const Line&, const Square&);
    friend bool test_collition (const Particle&, const Square&);
    friend bool test_collition (const Circle&, const Square&);
    friend bool test_collition (const Square&, const Square&);
    template<std::size_t N> friend bool test_collition (const Square&, const NEdge<N>&);

    friend void resolve_collition (Square&, Line&);
    friend void resolve_collition (Particle&, Square&);
    friend void resolve_collition (Circle&, Square&);
    friend void resolve_collition (Square&, Circle&);
    friend void resolve_collition (Square&, Square&);
    template<std::size_t N> friend void resolve_collition (Square&, NEdge<N>&);
    template<std::size_t N> friend void resolve_collition (NEdge<N>&, Square&);

    friend void correct_collition (Square&, Line&);
    friend void correct_collition (Particle&, Square&);
    friend void correct_collition (Circle&, Square&);
    friend void correct_collition (Square&, Circle&);
    friend void correct_collition (Square&, Square&);
    template<std::size_t N> friend void correct_collition (Square&, NEdge<N>&);
    template<std::size_t N> friend void correct_collition (NEdge<N>&, Square&);
};
