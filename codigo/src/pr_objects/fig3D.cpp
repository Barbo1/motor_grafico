#include "../../headers/pr_objects/fig3D.hpp"
#include "../../headers/concepts/texture.hpp"
#include <cstdint>
#include <utility>

fig3D::fig3D () {
  this->vertex = std::vector<Dir3> ();
  this->face_normal = std::vector<Dir3> ();
  this->face_index = std::vector<std::vector<uint32_t>> ();
  this->face_color = std::vector<SDL_Color> ();
  this->position = {0, 0, 0};
}

fig3D::fig3D (std::vector<std::vector<Dir3>> points) : fig3D() {
  this->position = {0, 0, 0};
  for (int i = 0; i < points.size(); i++) {

    /* face indeces. */
    face_index.push_back(std::vector<uint32_t>());
    std::vector<uint32_t>& index = this->face_index[i];
    
    /* normal. */
    Dir3 normal = Dir3();

    for (auto piter = points[i].begin(); piter < points[i].end(); piter++) {
      int j = 0;
      auto viter = vertex.begin();  
      while (j < vertex.size() && vertex[j] != *piter) { j++; }
      if (j == vertex.size())
        vertex.push_back (*piter);
      index.push_back (j);
      normal += *piter;
    }

    normal *= (1.f/points[i].size());
    face_normal.push_back (normal);
  }
}

fig3D::fig3D (const fig3D & fig) {
  this->position = fig.position;
  this->vertex = fig.vertex;
  this->face_normal = fig.face_normal;
  this->face_index = fig.face_index;
  this->face_color = fig.face_color;
}

fig3D::fig3D (fig3D && fig) {
  this->position = std::exchange(fig.position, Dir3());
  this->vertex = std::exchange(fig.vertex, std::vector<Dir3> ());
  this->face_normal = std::exchange(fig.face_normal, std::vector<Dir3> ());
  this->face_index = std::exchange(fig.face_index, std::vector<std::vector<uint32_t>> ());
  this->face_color = std::exchange(fig.face_color, std::vector<SDL_Color> ());
}

fig3D & fig3D::operator= (const fig3D & fig) {
  this->position = fig.position;
  this->vertex = fig.vertex;
  this->face_normal = fig.face_normal;
  this->face_index = fig.face_index;
  this->face_color = fig.face_color;
  return *this;
}

fig3D & fig3D::operator= (fig3D && fig) {
  this->position = std::exchange(fig.position, Dir3());
  this->vertex = std::exchange(fig.vertex, std::vector<Dir3> ());
  this->face_normal = std::exchange(fig.face_normal, std::vector<Dir3> ());
  this->face_index = std::exchange(fig.face_index, std::vector<std::vector<uint32_t>> ());
  this->face_color = std::exchange(fig.face_color, std::vector<SDL_Color> ());
  return *this;
}

bool fig3D::set_color (const std::vector<SDL_Color>& color) {
  if (color.size() != this->face_index.size())
    return false;

  this->face_color = color;
  return true;
}

void fig3D::rotate (const Dir3& rotation) {
  static auto all_rotate = [] (Dir3& vec, Dir3& coss, Dir3& sins) {
    float newitem = vec.z * sins.x + vec.y * coss.x;
    vec.z = vec.z * coss.x - vec.y * sins.x;
    vec.y = newitem;
    newitem = vec.x * coss.y - vec.z * sins.y;
    vec.z = vec.z * coss.y + vec.x * sins.y;
    vec.x = newitem;
    newitem = vec.x * coss.z + vec.y * sins.z;
    vec.y = vec.y * coss.z - vec.x * sins.z;
    vec.x = newitem;
  };

  Dir3 coss = {std::cos(rotation.x), std::cos(rotation.y), std::cos(rotation.z)};
  Dir3 sins = {std::sin(rotation.x), std::sin(rotation.y), std::sin(rotation.z)};

  for (auto& vec: this->vertex)
    all_rotate (vec, coss, sins);
  for (auto& vec: this->face_normal)
    all_rotate (vec, coss, sins);
}

void fig3D::set_position (const Dir3& position) {
  this->position = position;
}

void fig3D::move (const Dir3& displacement) {
  this->position += displacement;
}

void fig3D::draw (SDL_Renderer* render) {
  static std::vector<Dir2> points = std::vector<Dir2>();
  points.reserve(6);
  int i = 0;
  for (auto& ver_ind: this->face_index) {
    if (this->face_normal[i].z > 0) {
      for (auto& ind: ver_ind)
        points.push_back (this->vertex[ind] + this->position);
      print_polygon (render, points, face_color[i]);
      points.clear ();
    }
    i++;
  }
}
