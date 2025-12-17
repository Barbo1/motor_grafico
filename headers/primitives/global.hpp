#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <string>
#include <string_view>

#include "../concepts/lights.hpp"

class Global {
  private: 
    SDL_Window* window;
    SDL_Renderer* render;
    SDL_Texture* global_render_texture;
    SDL_Texture* render_mask;
    
    Uint32 clock_ticks;

    /* configuration. */
    uint32_t c_height, c_width;
    SDL_Color c_bg_color;
    std::string c_window_name;

    /* singleton. */
    static Global* single;
    Global (std::string_view window_name, uint32_t height, uint32_t width, SDL_Color bg_color) noexcept;
    
    SDL_Color darkness_color;
    
    /* random generator information. */
    static constexpr uint32_t relatives[32] {
      19, 8191, 337, 431, 89, 257, 2833, 353, 4649, 673, 193, 449, 9901, 97, 
      1777, 2113, 65537, 113, 953, 683, 1321, 26317, 37171, 1013, 1657, 1103, 
      13367, 5419, 1429, 20857, 241, 101
    };
    uint32_t c, a, xN;
  public:

    static Global* create (std::string_view window_name, uint32_t height, uint32_t width, SDL_Color bg_color) noexcept;
    ~Global ();

    /* Render functions. */
    void begin_render ();
    void end_render ();

    /* Internal information getters. */
    uint32_t get_height ();
    uint32_t get_width ();

    /* Wrapped information getters. */
    SDL_Window* get_window ();
    SDL_Renderer* get_render ();

    /* General getters. */
    float get_time ();
    uint32_t get_ticks ();
    uint32_t get_random ();
    float get_random_f01 ();

    /* Printing options. */
    void set_ambient_light (float per);
    void apply_mask (const ViewMask&);
};
