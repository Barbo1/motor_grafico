#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <vector>
#include <variant>
#include <string>

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

enum VisualType {
  D2FIG,
  D3FIG,
};

/* Textura. */
template <VisualType T>
class Visualizer {
  private: 
    int* use_count;
    SDL_Texture* texture;
    int height;
    int width;

    Visualizer(SDL_Renderer* render, int height, int width, Uint32* pixels);
    static Visualizer bounder (
      SDL_Renderer* render, int* bounds, int height, int width, SDL_Color color
    );

  public:
    Visualizer ();
    Visualizer (const Visualizer & texture);
    Visualizer (Visualizer && texture);
    Visualizer & operator= (const Visualizer & texture);
    Visualizer & operator= (Visualizer && texture);
    bool valid ();

    void draw (SDL_Renderer* render, const AngDir2 & position) const;

    /* special constructors */
    static Visualizer square (SDL_Renderer* render, int height, int base, SDL_Color color);
    static Visualizer rounded_square (SDL_Renderer* render, int height, int base, int radio, SDL_Color color);
    static Visualizer triangle (SDL_Renderer* render, Dir2 point1, Dir2 point2, Dir2 point3, SDL_Color color);
    static Visualizer circle (SDL_Renderer* render, int radio, SDL_Color color);
    static Visualizer circunference (SDL_Renderer* render, int radio, int width, SDL_Color color);
    static Visualizer oval (SDL_Renderer* render, int height, int base, SDL_Color color);
    static Visualizer polygon (SDL_Renderer* render, std::vector<Dir2> points, SDL_Color color);
    static Visualizer image (SDL_Renderer* render, std::string path);

    ~Visualizer();

    Visualizer<T> chargePNG (SDL_Renderer* render, const std::string& path);
    Visualizer<T> chargeJPEG (SDL_Renderer* render, const std::string& path);
    Visualizer<T> chargeBMP (SDL_Renderer* render, const std::string& path);
};

template class Visualizer<D2FIG>;

/* shows in the render the points in the array. */
void print_points (
  SDL_Renderer* render, std::vector<SDL_Point&> points, 
  SDL_Color stcol, SDL_Color ndcol, int radio, int division = 3
);

void print_polygon_c (SDL_Renderer* render, std::vector<Dir2> points, SDL_Color color);
void print_polygon_t (SDL_Renderer* render, std::vector<Dir2> points, const Visualizer<D2FIG>& texture);

void print_triangle_c (SDL_Renderer* render, Dir2 point1, Dir2 point2, Dir2 point3, SDL_Color color);
void print_triangle_t (SDL_Renderer* render, Dir2 point1, Dir2 point2, Dir2 point3, const Visualizer<D2FIG>& texture);

/* Tridimentional figures with only three verteces faces. */
template <>
class Visualizer<D3FIG> {
  private: 
    struct Face {
      uint64_t nor;
      uint64_t pos[3];
      uint64_t map[3];
    };

    struct Info {
      std::vector<Dir3> vectors;
      std::vector<Dir3> normals;
      std::vector<Dir2> mapping;
      std::vector<Face> indeces;
    };

    std::variant<Visualizer<D2FIG>, SDL_Color> texture;
    Info* info;
    int* use_count;

  public:
    Visualizer ();
    Visualizer (std::vector<std::vector<Dir3>> points, std::vector<Dir3> normals);
    Visualizer (std::string path);
    Visualizer (const Visualizer &);
    Visualizer (Visualizer &&);
    ~Visualizer ();
    Visualizer & operator= (const Visualizer &);
    Visualizer & operator= (Visualizer &&);

    void set_color (const SDL_Color& color);
    void set_texture (const Visualizer<D2FIG>& color);

    void rotate (const Dir3& rotation);

    void draw (SDL_Renderer* render, const Dir3 & position) const;
};
