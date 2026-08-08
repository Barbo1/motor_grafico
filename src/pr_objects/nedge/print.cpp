#include "../../../headers/pr_objects/nedge.hpp"
#include <codecvt>

void NEdge::print (Global * glb, GlyphsSystem * gs) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;

  for (uint32_t i = 0; i < this->size-2; i++) {
    const auto& triangle = this->placed_triangles[i];
    const Dir2 point1 = triangle[0];
    const Dir2 point2 = Dir2(triangle[1]) + point1;
    const Dir2 point3 = Dir2(triangle[2]) + point1;
    SDL_SetRenderDrawColor (glb->get_render(), 255, 0, 0, 255);
    SDL_RenderDrawLine (
      glb->get_render(), 
      static_cast<uint32_t>(point1.x()),
      static_cast<uint32_t>(point1.y()),
      static_cast<uint32_t>(point2.x()),
      static_cast<uint32_t>(point2.y())
    );
    SDL_RenderDrawLine (
      glb->get_render(), 
      static_cast<uint32_t>(point1.x()),
      static_cast<uint32_t>(point1.y()),
      static_cast<uint32_t>(point3.x()),
      static_cast<uint32_t>(point3.y())
    );
    SDL_RenderDrawLine (
      glb->get_render(), 
      static_cast<uint32_t>(point2.x()),
      static_cast<uint32_t>(point2.y()),
      static_cast<uint32_t>(point3.x()),
      static_cast<uint32_t>(point3.y())
    );
  }
  for (uint32_t i = 0; i < this->size; i++) {
    const auto& segment = this->placed_segments[i];
    gs->print(
      conv.from_bytes(std::to_string(i)), 
      20, 
      SDL_Color {.r = 255, .g = 255, .b = 255, .a = 255}, 
      Dir2(segment[1]) + Dir2(5.f, 5.f)
    );
  }
}
