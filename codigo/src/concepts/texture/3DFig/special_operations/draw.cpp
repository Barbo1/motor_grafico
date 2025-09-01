#include "../../../../../headers/concepts/texture.hpp"
#include <ranges>

void Visualizer<D3FIG>::draw (SDL_Renderer* render, const Dir3 & position) const {
  auto transformed = this->info->vectors 
    | std::views::transform([&](const Dir3& vec){ return vec + position; })
    | std::views::transform([&](const Dir3& vec){ return Dir2(vec); });
  std::vector<Dir2> vecs = std::vector<Dir2>(transformed.begin(), transformed.end());
  for (auto& face: this->info->indeces) {
    if (this->info->normals[face.nor].z > 0) {
      if (std::holds_alternative<SDL_Color>(this->texture))
        print_triangle_c (
          render, 
          vecs[face.pos[0]], 
          vecs[face.pos[1]], 
          vecs[face.pos[2]], 
          std::get<SDL_Color>(this->texture)
        );
      /*
      else
        print_polygon (render, points, std::get<Visualizer<D2FIG>>(this->texture));
      */
    }
  }
}
