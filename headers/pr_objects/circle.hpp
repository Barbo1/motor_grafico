#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/visualizer.hpp"
#include "../primitives/global.hpp"

class Line;
class Particle;
class Square;
template<std::size_t N> class NEdge;

#include <SDL2/SDL.h>
#include <cstdint>

class Square;
class Line;

class Circle: public Physical {
  private:
    uint32_t radio;
    Visualizer<D2FIG> texture;
    Global* glb;

  public: 
    Circle () noexcept;
    Circle (
      Global* glb, uint32_t radio, AngDir2 center, float density = 0, 
      float f_k = 0, bool movible = true, bool colidable = true, SDL_Color* color = nullptr
    ) noexcept;
    Circle (const Circle &) noexcept;
    Circle (Circle &&) noexcept;
    Circle & operator= (const Circle &) noexcept;
    Circle & operator= (Circle &&) noexcept;

    void set_texture (Visualizer<D2FIG>);
    void set_radio (uint32_t);

    Visualizer<D2FIG> get_texture () const;
    uint32_t get_radio () const;

    void draw ();

    friend bool test_collition (const Line&, const Circle&);
    friend bool test_collition (const Particle&, const Circle&);
    friend bool test_collition (const Circle&, const Circle&);
    friend bool test_collition (const Circle&, const Square&);
    template<std::size_t N> friend bool test_collition (const Circle&, const NEdge<N>&);

    friend void resolve_collition (Circle&, Line&);
    friend void resolve_collition (Particle&, Circle&);
    friend void resolve_collition (Circle&, Circle&);
    friend void resolve_collition (Circle&, Square&);
    friend void resolve_collition (Square&, Circle&);
    template<std::size_t N> friend void resolve_collition (Circle&, NEdge<N>&);
    template<std::size_t N> friend void resolve_collition (NEdge<N>&, Circle&);

    friend void correct_collition (Circle&, Line&);
    friend void correct_collition (Particle&, Circle&);
    friend void correct_collition (Circle&, Circle&);
    friend void correct_collition (Circle&, Square&);
    friend void correct_collition (Square&, Circle&);
    template<std::size_t N> friend void correct_collition (Circle&, NEdge<N>&);
    template<std::size_t N> friend void correct_collition (NEdge<N>&, Circle&);
};
