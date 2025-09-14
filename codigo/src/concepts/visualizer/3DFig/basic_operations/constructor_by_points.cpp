#include "../../../../../headers/concepts/visualizer.hpp"
#include <iostream>

Visualizer<D3FIG>::Visualizer (std::vector<std::vector<Dir3>> points, std::vector<Dir3> norms) {
  if (points.size() > norms.size()) {
    std::cout 
      << "Error: normals insuffitient for the point passed to create 3D figure." 
      << std::endl;
    std::exit (-1);
  }

  this->info = new Info {
    .vectors = std::vector<Dir3>(), 
    .normals = std::vector<Dir3>(),
    .mapping = std::vector<Dir2>({Dir2{0,0}}),
    .indeces = std::vector<Face>()
  };
  this->use_count = new int(1);
    
  this->info->indeces.reserve (20);
  this->info->vectors.reserve (20);
  int j, k;
  for (int i = 0; i < points.size(); i++) {
   
    this->info->indeces.emplace_back(
      Face {
        .nor = 0, 
        .pos = {}, 
        .map = {0,0,0}
      }
    );
    auto& face = this->info->indeces.back ();

    j = 0;
    auto piter = points[i].begin();
    for (; j < 3; piter++, j++) {
      k = 0;
      while (k < this->info->vectors.size() && this->info->vectors[k] != *piter) { k++; }
      if (k == this->info->vectors.size())
        this->info->vectors.push_back (*piter);
      face.pos[j] = k;
    }

    Dir3 normal = norms[i];
    k = 0;
    while (k < this->info->normals.size() && this->info->normals[k] != normal) { k++; }
    if (k == this->info->normals.size())
      this->info->normals.push_back (normal);
    face.nor = k;
  }
}
