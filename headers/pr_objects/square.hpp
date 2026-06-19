#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/visualizer.hpp"
#include "../primitives/global.hpp"
#include <cstdint>

class Circle;
class Line;
class Particle;
template<std::size_t N> class NEdge;

class Square final: public Physical {
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

    friend bool test_collision (const Line&, const Square&);
    friend bool test_collision (const Particle&, const Square&);
    friend bool test_collision (const Circle&, const Square&);
    friend bool test_collision (const Square&, const Square&);
    template<std::size_t N> friend bool test_collision (const Square&, const NEdge<N>&);

    friend void resolve_collision (Square&, Line&);
    friend void resolve_collision (Particle&, Square&);
    friend void resolve_collision (Circle&, Square&);
    friend void resolve_collision (Square&, Circle&);
    friend void resolve_collision (Square&, Square&);
    template<std::size_t N> friend void resolve_collision (Square&, NEdge<N>&);
    template<std::size_t N> friend void resolve_collision (NEdge<N>&, Square&);

    friend void correct_collision (Square&, Line&);
    friend void correct_collision (Particle&, Square&);
    friend void correct_collision (Circle&, Square&);
    friend void correct_collision (Square&, Circle&);
    friend void correct_collision (Square&, Square&);
    template<std::size_t N> friend void correct_collision (Square&, NEdge<N>&);
    template<std::size_t N> friend void correct_collision (NEdge<N>&, Square&);
  
    friend Dir2 collision_point (const Line&, const Square&);
    friend Dir2 collision_point (const Circle&, const Square&);
    friend Dir2 collision_point (const Square&, const Square&);
    template<std::size_t N> friend Dir2 collision_point (const Square&, const NEdge<N>&);
};
