#pragma once

#include "../concepts/physical.hpp"
#include "../concepts/visualizer.hpp"
#include "../primitives/global.hpp"

class Circle;
class Line;
class Particle;
class Square;

#include <array>

template<std::size_t N>
class NEdge: public Physical {
  private:
    std::array<Dir2, N> points;
    std::array<std::array<Dir2, 3>, N-2> triangles;
    Visualizer<D2FIG> texture;
    Global* glb;

  public: 
    NEdge () noexcept;
    NEdge (Global* glb, const std::vector<Dir2> & points, SDL_Color* color = nullptr, int* error = nullptr) noexcept;
    NEdge (const NEdge &) noexcept;
    NEdge (NEdge &&) noexcept;
    NEdge & operator= (const NEdge &) noexcept;
    NEdge & operator= (NEdge &&) noexcept;

    void set_texture (Visualizer<D2FIG>);
    Visualizer<D2FIG> get_texture () const;

    void draw ();

    template<std::size_t M> friend bool test_collition (const Line&, const NEdge<M>&);
    template<std::size_t M> friend bool test_collition (const Particle&, const NEdge<M>&);
    template<std::size_t M> friend bool test_collition (const Circle&, const NEdge<M>&);
    template<std::size_t M> friend bool test_collition (const Square&, const NEdge<M>&);
    template<std::size_t M, std::size_t L> friend bool test_collition (const NEdge<M>&, const NEdge<L>&);

    template<std::size_t M> friend void resolve_collition (NEdge<N>&, Line&);
    template<std::size_t M> friend void resolve_collition (Particle&, NEdge<N>&);
    template<std::size_t M> friend void resolve_collition (Circle&, NEdge<N>&);
    template<std::size_t M> friend void resolve_collition (Square&, NEdge<N>&);
    template<std::size_t M> friend void resolve_collition (NEdge<N>&, Circle&);
    template<std::size_t M> friend void resolve_collition (NEdge<N>&, Square&);
    template<std::size_t M, std::size_t T> friend void resolve_collition (NEdge<N>&, NEdge<M>&);

    template<std::size_t M> friend void correct_collition (NEdge<N>&, Line&);
    template<std::size_t M> friend void correct_collition (Particle&, NEdge<N>&);
    template<std::size_t M> friend void correct_collition (Circle&, NEdge<N>&);
    template<std::size_t M> friend void correct_collition (Square&, NEdge<N>&);
    template<std::size_t M> friend void correct_collition (NEdge<N>&, Circle&);
    template<std::size_t M> friend void correct_collition (NEdge<N>&, Square&);
    template<std::size_t M, std::size_t T> friend void correct_collition (NEdge<N>&, NEdge<M>&);
};
