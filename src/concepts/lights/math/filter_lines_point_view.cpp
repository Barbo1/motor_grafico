#include "../../../../headers/concepts/lights.hpp"
#include "../../../../headers/primitives/math.hpp"

#include <immintrin.h>
#include <cstdint>
#include <cmath>

MaskObjectList filter_lines_point_view (
  DynamicalArena& darena,
  const MaskObjectList& segments,
  const Light& light, 
  const Dir2 screen_dims
) {
  auto [K, dimsK] = find_light_bounding (light, screen_dims);
  Dir2 coefs;
  uint32_t size = 0;
  MaskObject* ret = nullptr;
  MaskObject* ret_iter = nullptr;
  for (MaskObject* iter = segments.obj; iter != nullptr; iter = iter->next) {
    Dir2 p = Dir2(iter->point1);
    Dir2 v = Dir2(iter->point2) - p;

    if (test_collision_square_segment (K, dimsK, p, v, coefs)) {
      MaskObject* aux = darena.alloc_mo();
      aux->point1.store(v.madd(coefs.x(), p));
      aux->point2.store(v.madd(coefs.y(), p));

      if (ret == nullptr) {
        ret = ret_iter = aux;
      } else {
        ret_iter->next = aux;
        ret_iter = aux;
      }
      
      size++;
    }
  }
  
  if (ret != nullptr)
    ret_iter->next = nullptr;

  /* appending bounds. */
  Dir2 D = K + dimsK;
  Dir2 I = K - dimsK;
  Dir2 P1 = K + dimsK.dir_mul(Dir2(-1.f, 1.f));
  Dir2 P2 = K - dimsK.dir_mul(Dir2(-1.f, 1.f));

  MaskObject* elem = darena.alloc_mo();
  elem->point1.store(D);
  elem->point2.store(P1);
  elem->next = ret;
  ret = elem;

  elem = darena.alloc_mo();
  elem->point1.store(D);
  elem->point2.store(P2);
  elem->next = ret;
  ret = elem;
  
  elem = darena.alloc_mo();
  elem->point1.store(I);
  elem->point2.store(P1);
  elem->next = ret;
  ret = elem;

  elem = darena.alloc_mo();
  elem->point1.store(I);
  elem->point2.store(P2);
  elem->next = ret;
  ret = elem;

  return MaskObjectList {
    .obj = ret, 
    .size = size + 4
  };
}
