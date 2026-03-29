#pragma once

#include "../concepts/physical.hpp"
#include "nedge.hpp"

class Circle;
class Particle;
class Square;
template<std::size_t N> class NEdge;

#include <SDL2/SDL.h>

class Circle;
class Square;

class Line {
  private:
    Dir2 v;
    Dir2 p;

  public: 
    Line () noexcept;
    Line (float slope, float deviation) noexcept;
    Line (Dir2 p1, Dir2 p2) noexcept;
    Line (const Line &) noexcept;
    Line (Line &&) noexcept;
    Line & operator= (const Line &) noexcept;
    Line & operator= (Line &&) noexcept;

    void set_v (Dir2);
    void set_p (Dir2);

    float get_slope () const;
    float get_deviation () const;
    Dir2 get_v () const;
    Dir2 get_p () const;

    friend bool test_collition (const Line&, Physical&);
    friend bool test_collition (const Line&, const Particle&);
    friend bool test_collition (const Line&, const Circle&);
    friend bool test_collition (const Line&, const Square&);
    template<std::size_t N> friend bool test_collition (const Line&, const NEdge<N>&);

    friend void resolve_collition (Physical&, Line&);
    friend void resolve_collition (Particle&, Line&);
    friend void resolve_collition (Circle&, Line&);
    friend void resolve_collition (Square&, Line&);
    template<std::size_t N> friend void resolve_collition (NEdge<N>&, Line&);

    friend void correct_collition (Physical&, Line&);
    friend void correct_collition (Particle&, Line&);
    friend void correct_collition (Square&, Line&);
    friend void correct_collition (Circle&, Line&);
    template<std::size_t N> friend void correct_collition (Line&, NEdge<N>&);
};
