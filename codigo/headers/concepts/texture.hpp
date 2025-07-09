#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <vector>
#include <array>

#include "./primitives.hpp"

/*
 * hay que(entre varias texturas):
 *  - juntar dos texturas -> append
 *  - tomar la direrencia -> differ
 *  - tomar todo lo de una y quitar lo de la otra -> abstain
 *  - tomar todo lo de una y agregar de la otra solo lo que se solape -> overlap
 *
 * hay que(dentro de una textura):
 *  - añadir nuevos nivieles -> add_level
 *  - añadir un punto a un nivel(en orden) -> add_point
 *  - añadir puntos a un nivel(en orden) -> add_point
 *  - juntar los puntos que hay en dos indices y colocarlo en uno solo(con las propiedades del primer indice) -> join_level
 *
 * es mejor si:
 *  - todos los puntos estan correctamente ordenados (baja orden algoritmico)
 */

/* shows in the render the points in the array. */
void print_points (
  SDL_Renderer* render, std::vector<SDL_Point&> points, 
  SDL_Color stcol, SDL_Color ndcol, int radio, int division = 3
);

class Texture {
  private: 
    int* use_count;
    SDL_Texture* texture;
    SDL_Point center;
    int height;
    int width;

    static constexpr std::array<uint64_t, 64> powers {
      1ULL,                   2ULL,                   4ULL,                   8ULL,
      16ULL,                  32ULL,                  64ULL,                  128ULL,
      256ULL,                 512ULL,                 1024ULL,                2048ULL,
      4096ULL,                8192ULL,                16384ULL,               32768ULL,
      65536ULL,               131072ULL,              262144ULL,              524288ULL,
      1048576ULL,             2097152ULL,             4194304ULL,             8388608ULL,
      16777216ULL,            33554432ULL,            67108864ULL,            134217728ULL,
      268435456ULL,           536870912ULL,           1073741824ULL,          2147483648ULL,
      4294967296ULL,          8589934592ULL,          17179869184ULL,         34359738368ULL,
      68719476736ULL,         137438953472ULL,        274877906944ULL,        549755813888ULL,
      1099511627776ULL,       2199023255552ULL,       4398046511104ULL,       8796093022208ULL,
      17592186044416ULL,      35184372088832ULL,      70368744177664ULL,      140737488355328ULL,
      281474976710656ULL,     562949953421312ULL,     1125899906842624ULL,    2251799813685248ULL,
      4503599627370496ULL,    9007199254740992ULL,    18014398509481984ULL,   36028797018963968ULL,
      72057594037927936ULL,   144115188075855872ULL,  288230376151711744ULL,  576460752303423488ULL,
      1152921504606846976ULL, 2305843009213693952ULL, 4611686018427387904ULL, 9223372036854775808ULL
    };

    Texture(SDL_Renderer* render, int height, int width, SDL_Point & center, Uint32* pixels);
    static Texture bounder (
      SDL_Renderer* render, int* bounds, int height, int width, SDL_Color color, SDL_Point center
    );

  public:
    Texture ();
    Texture (const Texture & texture);
    Texture (Texture && texture);
    Texture & operator= (const Texture & texture);
    Texture & operator= (Texture && texture);

    void draw (SDL_Renderer* render, const AngularDirection & position) const;

    void set_center (Direction center);

    /* special constructors */
    static Texture square (SDL_Renderer* render, int height, int base, SDL_Color color);
    static Texture rounded_square (SDL_Renderer* render, int height, int base, int radio, SDL_Color color);
    static Texture triangle (SDL_Renderer* render, Direction point1, Direction point2, Direction point3, SDL_Color color);
    static Texture circle (SDL_Renderer* render, int radio, SDL_Color color);
    static Texture circunference (SDL_Renderer* render, int radio, int width, SDL_Color color);
    static Texture oval (SDL_Renderer* render, int height, int base, SDL_Color color);
    static Texture polygon (SDL_Renderer* render, std::vector<Direction> points, SDL_Color color);

    ~Texture();
};
