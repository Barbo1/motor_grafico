#include "../../../headers/primitives/math.hpp"
#include <array> 

void rotate_triangles (std::array<Dir2, 3>* triangles, uint32_t many, float angle) {
  float sin = std::sin(angle);
  float cos = std::cos(angle);
  Dir2 v_x = Dir2(cos, sin);
  Dir2 v_y = Dir2(-sin, cos);
  for (uint32_t i = 0; i < many; i++) {
    std::array<Dir2, 3>& triangle = triangles[i];
    Dir2& point = triangle[0]; 
    Dir2& vec1 = triangle[1]; 
    Dir2& vec2 = triangle[2]; 
    point = v_x * point.x + v_y * point.y;
    vec1 = v_x * vec1.x + v_y * vec1.y;
    vec2 = v_x * vec2.x + v_y * vec2.y;
  }
}
