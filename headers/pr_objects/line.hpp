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
    MemDir2 v;
    MemDir2 p;

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

    friend bool test_collision (const Line&, Physical&);
    friend bool test_collision (const Line&, const Particle&);
    friend bool test_collision (const Line&, const Circle&);
    friend bool test_collision (const Line&, const Square&);
    template<std::size_t N> friend bool test_collision (const Line&, const NEdge<N>&);

    friend void resolve_collision (Physical&, Line&);
    friend void resolve_collision (Particle&, Line&);
    friend void resolve_collision (Circle&, Line&);
    friend void resolve_collision (Square&, Line&);
    template<std::size_t N> friend void resolve_collision (NEdge<N>&, Line&);

    friend void correct_collision (Physical&, Line&);
    friend void correct_collision (Particle&, Line&);
    friend void correct_collision (Square&, Line&);
    friend void correct_collision (Circle&, Line&);
    template<std::size_t N> friend void correct_collision (Line&, NEdge<N>&);
    
    Dir2 collision_point (const Line&, const Physical&);
    Dir2 collision_point (const Line&, const Square&);
    Dir2 collision_point (const Line&, const Circle&);
    template<std::size_t N> Dir2 collision_point (const Line&, const NEdge<N>&);
};
