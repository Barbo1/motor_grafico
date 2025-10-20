#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <string>

class Global {
  private: 
    SDL_Window* window;
    SDL_Renderer* render;
    SDL_Texture* global_render_texture;
    Uint64 clock_ticks;

    /* configuration. */
    uint32_t c_height, c_width;
    SDL_Color c_bg_color;
    std::string c_window_name;

    /* singleton. */
    static Global* single;
    Global (char* window_name, uint32_t height, uint32_t width, SDL_Color bg_color);

  public:
    static Global* create (char* window_name, uint32_t height, uint32_t width, SDL_Color bg_color);
    ~Global ();

    void begin_render ();
    void end_render ();

    uint32_t get_height ();
    uint32_t get_width ();

    SDL_Renderer* get_render ();
    float get_time ();
};
