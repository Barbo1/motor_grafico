#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <cstdint>
#include <vector>
#include <string>

#include "../primitives/vectors.hpp"
#include "./visualizer.hpp"

class ImageModifier {
  private: 
    SDL_Surface* texture;
    
    ImageModifier (int height, int width, Uint32* pixels);
    static ImageModifier bounder (int* bounds, int height, int width, SDL_Color color);

  public:
    ImageModifier ();
    ImageModifier (const ImageModifier & texture);
    ImageModifier (ImageModifier && texture);
    ImageModifier & operator= (const ImageModifier & texture);
    ImageModifier & operator= (ImageModifier && texture);
    ~ImageModifier ();

    Visualizer<D2FIG> cast (SDL_Renderer* render);
    SDL_Color operator() (uint32_t, uint32_t);

    /* Modifications. */
    ImageModifier& resize (uint32_t hieght, uint32_t width);
    ImageModifier& shift (const Dir2& offset);
    ImageModifier& xshear (float angle);
    ImageModifier& xmirror ();
    ImageModifier& ymirror ();
    ImageModifier& rotate90 ();
    ImageModifier& rotate180 ();
    ImageModifier& rotate270 ();

    ImageModifier& operator| (const ImageModifier& img); /* Union. */
    ImageModifier& operator& (const ImageModifier& img); /* Intersection. */
    ImageModifier& operator- (const ImageModifier& img); /* Difference. */
    ImageModifier& operator* (const ImageModifier& img); /* Overlap. */

    ImageModifier& rbpp (uint32_t number_reduction); /* reduce bits per pixel. */
    ImageModifier& apply (Uint32 (*fun)(Uint32));
    ImageModifier& project (Dir2 point1, Dir2 point2, Dir2 point3, Dir2 point4);

    /* Special constructors. */
    static ImageModifier square (int height, int base, SDL_Color color);
    static ImageModifier rounded_square (int height, int base, int radio, SDL_Color color);
    static ImageModifier triangle (Dir2 point1, Dir2 point2, Dir2 point3, SDL_Color color);
    static ImageModifier circle (int radio, SDL_Color color);
    static ImageModifier circunference (int radio, int width, SDL_Color color);
    static ImageModifier oval (int height, int base, SDL_Color color);
    static ImageModifier polygon (std::vector<Dir2> points, SDL_Color color);

    static ImageModifier chargePNG (const std::string& path);
};
