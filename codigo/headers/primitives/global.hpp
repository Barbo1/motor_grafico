#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <string>
#include <string_view>

class Global {
  private: 
    SDL_Window* window;
    SDL_Renderer* render;
    SDL_Texture* global_render_texture;
    
    Uint32 clock_ticks;

    /* configuration. */
    uint32_t c_height, c_width;
    SDL_Color c_bg_color;
    std::string c_window_name;

    /* singleton. */
    static Global* single;
    Global (std::string_view window_name, uint32_t height, uint32_t width, SDL_Color bg_color);
    
    /* random generator information. */
    static constexpr uint32_t relatives[32] {
      19, 8191, 337, 431, 89, 257, 2833, 353, 4649, 673, 193, 449, 9901, 97, 
      1777, 2113, 65537, 113, 953, 683, 1321, 26317, 37171, 1013, 1657, 1103, 
      13367, 5419, 1429, 20857, 241, 101
    };
    uint32_t c, a, xN;
  public:

    static Global* create (std::string_view window_name, uint32_t height, uint32_t width, SDL_Color bg_color);
    ~Global ();

    void begin_render ();
    void end_render ();

    uint32_t get_height ();
    uint32_t get_width ();

    SDL_Window* get_window ();
    SDL_Renderer* get_render ();

    float get_time ();
    uint32_t get_ticks ();
    uint32_t get_random ();
    float get_random_f01 ();
};
