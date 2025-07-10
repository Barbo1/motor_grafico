#include "../concepts/primitives.hpp"
#include <vector>
#include <SDL2/SDL.h>
#include <cstdint>

class fig3D {
  private: 
    std::vector<Dir3> vertex;
    std::vector<Dir3> face_normal;
    std::vector<std::vector<uint32_t>> face_index;
    std::vector<SDL_Color> face_color;

    Dir3 position;

  public:
    fig3D ();
    fig3D (std::vector<std::vector<Dir3>> points);
    fig3D (const fig3D &);
    fig3D (fig3D &&);
    fig3D & operator= (const fig3D &);
    fig3D & operator= (fig3D &&);

    void rotate (const Dir3& rotation);
    void move (const Dir3& displacement);

    void set_position (const Dir3& position);
    bool set_color (const std::vector<SDL_Color>& color);

    void draw (SDL_Renderer* render);
};
