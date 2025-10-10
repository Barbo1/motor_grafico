#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <cstdint>
#include <vector>
#include <variant>
#include <string>

#include "../primitives/vectors.hpp"

/* Shows in the render the points in the array. */
void print_points (
  SDL_Renderer* render, std::vector<SDL_Point&> points, 
  SDL_Color stcol, SDL_Color ndcol, int radio, int division = 3
);

/* Shows in the render the line between the two points. */
void print_line (
  SDL_Renderer* render, SDL_Point point1, SDL_Point point2, SDL_Color color
);

/* Functions meant to print plain bidimentional figures. */
void print_polygon_c (SDL_Renderer* render, std::vector<Dir2> points, SDL_Color color);
void print_triangle_c (SDL_Renderer* render, Dir2 point1, Dir2 point2, Dir2 point3, SDL_Color color);

enum VisualType {
  D2FIG,
  D3FIG,
};

class ImageModifier;

/* This kind of Visualizer represents a bidimentional texture. */
template <VisualType T>
class Visualizer {
  private: 
    int* use_count;
    SDL_Texture* texture;
    int height;
    int width;

    Visualizer(SDL_Renderer* render, int height, int width, Uint32* pixels);

  public:
    Visualizer ();
    Visualizer (const Visualizer & texture);
    Visualizer (Visualizer && texture);
    Visualizer & operator= (const Visualizer & texture);
    Visualizer & operator= (Visualizer && texture);
    ~Visualizer();

    void draw (SDL_Renderer* render, const AngDir2 & position) const;

    friend ImageModifier;
    friend Visualizer<D2FIG> chargePNG (SDL_Renderer* render, const std::string& path);
};

template class Visualizer<D2FIG>;

Uint32* charging_PNG_to_memory (const std::string& path, int & width, int & height);

Visualizer<D2FIG> chargePNG (SDL_Renderer* render, const std::string& path);

/* Function meant to print a bidimentional figures with a uv mapping. */
void print_polygon_t (
  SDL_Renderer* render, 
  std::vector<Dir2> points, 
  std::vector<Dir2> uvs, 
  const Visualizer<D2FIG>& texture
);

void print_triangle_t (
  SDL_Renderer* render, 
  Dir2 point1, Dir2 point2, Dir2 point3, 
  Dir2 uv1, Dir2 uv2, Dir2 uv3, 
  SDL_Surface* texture
);

/* This kind of Visualizer represents a tridimentional figures, posibly with uv mapping. */
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

    std::variant<SDL_Surface*, SDL_Color> texture;
    Info* info;
    int* use_count;

  public:
    Visualizer ();
    Visualizer (std::vector<std::vector<Dir3>> points, std::vector<Dir3> normals);
    Visualizer (std::string path, std::string img = "", bool uv_hiding = false);
    Visualizer (const Visualizer &);
    Visualizer (Visualizer &&);
    Visualizer & operator= (const Visualizer &);
    Visualizer & operator= (Visualizer &&);
    ~Visualizer ();

    void set_color (const SDL_Color& color);
    void set_texture (const std::string & img);

    void rotate (const Dir3& rotation);
    void resize (float coef);

    void draw (SDL_Renderer* render, const Dir3 & position) const;
    
    /* special constructors */
    static Visualizer prism (SDL_Renderer* render, float base, float height, float depth);
    static Visualizer pyramid (SDL_Renderer* render, float height, float base_rad, int base_vert);
};
