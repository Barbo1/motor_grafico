#include "../../../../../headers/concepts/lights.hpp"
#include "../../../../../headers/primitives/math.hpp"

#include <cmath>
#include <cstdint>
#include <immintrin.h>

MaskObjectList filter_lines_point_view (
  DynamicalArena& darena,
  MaskObjectList segments,
  const Light& light, 
  const Dir2 screen_pos, 
  const Dir2 screen_dims
) {
  float light_bound = (2.f * light.intensity - 1.f) / light.attenuation;
  Dir2 light_dims = Dir2(light_bound, light_bound);
  Dir2 light_pos = light.position;

  Dir2 coefs;
  uint32_t size = 0;
  MaskObject* ret = nullptr;
  MaskObject* ret_iter = nullptr;
  for (MaskObject* iter = segments.obj; iter != nullptr; iter = iter->next) {
    Dir2 p = Dir2(iter->point1);
    Dir2 v = Dir2(iter->point2) - p;

    if (test_collision_square_segment (screen_pos, screen_dims, p, v, coefs)) {
      float x = coefs.x();
      p = v.madd(x, p);
      v = v*(coefs.y() - x);
    } else {
      continue;
    }

    if (test_collision_square_segment (light_pos, light_dims, p, v, coefs)) {
      float x = coefs.x();
      p = v.madd(x, p);
      v = v*(coefs.y() - x);
    } else {
      continue;
    }

    MaskObject* aux = darena.alloc_mo();
    if (ret == nullptr) {
      ret = ret_iter = aux;
    } else {
      ret_iter->next = aux;
      ret_iter = aux;
    }

    ret_iter->point1.store(p);
    ret_iter->point2.store(v + p);
    size++;
  }
  
  if (ret != nullptr)
    ret_iter->next = nullptr;

  return MaskObjectList {
    .obj =ret, 
    .size = size
  };
}
