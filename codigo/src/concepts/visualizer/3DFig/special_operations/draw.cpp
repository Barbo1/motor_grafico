#include "../../../../../headers/concepts/visualizer.hpp"
#include <ranges>

void Visualizer<D3FIG>::draw (SDL_Renderer* render, const Dir3 & position) const {
  auto transformed = this->info->vectors 
    | std::views::transform([&] (const Dir3& vec) { return Dir2(vec + position); });
  auto iterable = this->info->indeces 
    | std::views::filter([&] (const Face& face) { return this->info->normals[face.nor].z > 0.0f; });
  if (std::holds_alternative<SDL_Color>(this->texture)) {
    for (auto& face: iterable) {
      print_triangle_c (
        render,
        transformed[face.pos[0]],
        transformed[face.pos[1]],
        transformed[face.pos[2]],
        std::get<SDL_Color>(this->texture)
      );
    }
  } else {
    for (auto& face: iterable) {
      print_triangle_t (
        render,
        transformed[face.pos[0]],
        transformed[face.pos[1]],
        transformed[face.pos[2]],
        this->info->mapping[face.map[0]],
        this->info->mapping[face.map[1]],
        this->info->mapping[face.map[2]],
        std::get<SDL_Surface*>(this->texture)
      );
    }
  }
}
