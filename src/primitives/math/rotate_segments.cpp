#include "../../../headers/primitives/math.hpp"

void rotate_segments (std::pair<Dir2, Dir2>* segments, uint32_t many, float angle) {
  float sin = std::sin(angle);
  float cos = std::cos(angle);
  Dir2 v_x = Dir2(cos, sin);
  Dir2 v_y = Dir2(-sin, cos);
  for (uint32_t i = 0; i < many; i++) {
    std::pair<Dir2, Dir2>& segment = segments[i];
    segment.first = v_x * segment.first.x + v_y * segment.first.y;
    segment.second = v_x * segment.second.x + v_y * segment.second.y;
  }
}
