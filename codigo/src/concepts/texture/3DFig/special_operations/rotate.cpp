#include "../../../../../headers/concepts/texture.hpp"

void Visualizer<D3FIG>::rotate (const Dir3& rotation) {
  static auto all_rotate = [] (Dir3& vec, const Dir3& coss, const Dir3& sins) {
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

  const Dir3 coss = {std::cos(rotation.x), std::cos(rotation.y), std::cos(rotation.z)};
  const Dir3 sins = {std::sin(rotation.x), std::sin(rotation.y), std::sin(rotation.z)};

  for (auto& vec: this->info->vectors)
    all_rotate (vec, coss, sins);
  for (auto& vec: this->info->normals)
    all_rotate (vec, coss, sins);
}
