#include "../../../../../headers/concepts/texture.hpp"
#include <ranges>

void Visualizer<D3FIG>::draw (SDL_Renderer* render, const Dir3 & position) const {
  auto transformed = this->info->vectors 
    | std::views::transform([&] (const Dir3& vec) { return Dir2(vec + position); });
  std::vector<Dir2> vecs = std::vector<Dir2>(transformed.begin(), transformed.end());
  int i = 0, j = 0;
  for (auto& face: this->info->indeces) {
    if (this->info->normals[face.nor].z > 0.0f) {
      if (std::holds_alternative<SDL_Color>(this->texture))
        print_triangle_c (
          render,
          vecs[face.pos[0]],
          vecs[face.pos[1]],
          vecs[face.pos[2]],
          std::get<SDL_Color>(this->texture)
        );
      else print_triangle_t (
        render,
        vecs[face.pos[0]],
        vecs[face.pos[1]],
        vecs[face.pos[2]],
        this->info->mapping[face.map[0]],
        this->info->mapping[face.map[1]],
        this->info->mapping[face.map[2]],
        std::get<SDL_Surface*>(this->texture)
      );
    }
  }
}
