#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <cstdint>
#include <vector>
#include <string>
#include "./primitives.hpp"
#include "./visualizer.hpp"

/*
 * hay que(entre varias texturas bidimencionales):
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

class ImageModifier {
  private: 
    SDL_Surface* texture;
    
    ImageModifier (SDL_Renderer* render, Uint32* pixels);

  public:
    ImageModifier ();
    ImageModifier (SDL_Renderer* render, std::string path);
    ImageModifier (const ImageModifier & texture);
    ImageModifier (ImageModifier && texture);
    ImageModifier & operator= (const ImageModifier & texture);
    ImageModifier & operator= (ImageModifier && texture);
    ~ImageModifier ();

    Visualizer<D2FIG> cast ();
    SDL_Color operator() (uint32_t, uint32_t);

    /* Modifications. */
    ImageModifier& resize (uint32_t hieght, uint32_t width);
    ImageModifier& shift (const Dir2& offset);
    ImageModifier& xshear (float angle);
    ImageModifier& yshear (float angle);
    ImageModifier& rotate90 ();
    ImageModifier& rotate180 ();
    ImageModifier& rotate270 ();

    ImageModifier& operator| (const ImageModifier& img); /* Union. */
    ImageModifier& operator& (const ImageModifier& img); /* Intersection. */
    ImageModifier& operator- (const ImageModifier& img); /* Difference. */
    ImageModifier& operator* (const ImageModifier& img); /* Overlap. */

    ImageModifier& rbpp (uint32_t number_reduction); /* reduce bits per pixel. */
    ImageModifier& apply (Uint32 (*fun)(Uint32));
    ImageModifier& proyect (Dir2 point1, Dir2 point2, Dir2 point3, Dir2 point4);

    /* Special constructors. */
    static ImageModifier square (SDL_Renderer* render, int height, int base, SDL_Color color);
    static ImageModifier rounded_square (SDL_Renderer* render, int height, int base, int radio, SDL_Color color);
    static ImageModifier triangle (SDL_Renderer* render, Dir2 point1, Dir2 point2, Dir2 point3, SDL_Color color);
    static ImageModifier circle (SDL_Renderer* render, int radio, SDL_Color color);
    static ImageModifier circunference (SDL_Renderer* render, int radio, int width, SDL_Color color);
    static ImageModifier oval (SDL_Renderer* render, int height, int base, SDL_Color color);
    static ImageModifier polygon (SDL_Renderer* render, std::vector<Dir2> points, SDL_Color color);
    static ImageModifier image (SDL_Renderer* render, std::string path);

    static ImageModifier chargePNG (SDL_Renderer* render, const std::string& path);
    static ImageModifier chargeJPEG (SDL_Renderer* render, const std::string& path);
    static ImageModifier chargeBMP (SDL_Renderer* render, const std::string& path);
};
