#pragma once

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <vector>

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

class Texture {
  private: 
    int* use_count;
    SDL_Texture* texture;
    SDL_Point center;
    int height;
    int width;

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
    static Texture triangle (SDL_Renderer* render, Direction point1, Direction point2, Direction point3, SDL_Color color);
    static Texture circle (SDL_Renderer* render, int radio, SDL_Color color);
    static Texture oval (SDL_Renderer* render, int height, int base, SDL_Color color);
    static Texture polygon (SDL_Renderer* render, std::vector<Direction> points, SDL_Color color);

    ~Texture();
};
