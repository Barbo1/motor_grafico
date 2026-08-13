#include "../../../../../headers/concepts/lights.hpp"
#include "../../../../../headers/primitives/math.hpp"
#include <SDL2/SDL_render.h>
#include <cstdint>

#include <cstdio>
inline void print_m128(__m128 v)
{
    alignas(16) float f[4];
    _mm_store_ps(f, v);

    std::printf("[%.6f, %.6f, %.6f, %.6f]\n",
                f[0], f[1], f[2], f[3]);
}

ViewMask& ViewMask::draw_light_view_mask (
  DynamicalArena& darena,
  const std::span<std::pair<Light, MaskObjectList>, std::dynamic_extent>& segments,
  const Dir2& screen_dims 
) {
  Uint32* buffer = (Uint32*)img->pixels;
  std::vector<MaskObjectList> new_segments(segments.size());

  for (uint32_t n = 0; n < segments.size(); n++) {
    const Light& light = segments[n].first;
    const MaskObjectList& segment = segments[n].second;

    Dir2 light_pos = Dir2(light.position);
    MaskObjectList filtered = filter_lines_point_view(darena, segment, light, screen_dims);
    MaskObjectList viewed = generate_view_covering (
      darena,
      filtered, 
      light_pos, 
      ViewGeneration::POINT
    );
    darena.complete_free_mo(filtered.obj);

    if (viewed.obj != nullptr) {
      for (MaskObject* iter = viewed.obj; iter != nullptr; iter = iter->next) {
        iter->point1.store(Dir2(iter->point1) - light_pos);
        iter->point2.store(Dir2(iter->point2) - light_pos);
      }
    }
    new_segments[n] = viewed;
  }
 
  __m128i mm_opr_mask = _mm_set_epi8 (0,0,0,0,0,0,0,0,0,0,0,0,12,8,4,0);

  Dir2 pos = Dir2();
  Dir2 advance_x = Dir2(1.f, 0.f);
  Dir2 advance_y = Dir2(0.f, 1.f);
  Dir2 aux_advance_y = Dir2(0.f, 1.f);
  __m128 ext_255 = _mm_set1_ps(255.f);

  Uint32* buffer_iter = buffer;
  for (uint32_t h = 0; h < (uint32_t)img->h; h++) {
    for (uint32_t w = 0; w < (uint32_t)img->w; w++) {
      __m128 curr_color = _mm_setzero_ps();
      __m128 curr_alpha = _mm_set1_ps(255.f);

      for (uint32_t j = 0; j < segments.size(); j++) {
        const Light& light = segments[j].first;

        Dir2 light_pos = Dir2(light.position);
        MaskObject* iter = new_segments[j].obj;
        for (uint32_t k = 0; k < new_segments[j].size; k++) {
          if (test_collision_triangle_point (light_pos, Dir2(iter->point1), Dir2(iter->point2), pos)) {
              
            __m128 inv_intensity = _mm_rcp_ps (_mm_set1_ps (light.intensity));
            __m128 denom_part = _mm_mul_ps (_mm_set1_ps (light.attenuation), _mm_sub_ps (pos.v, light_pos.v));
            denom_part = _mm_mul_ps (denom_part, denom_part);
            denom_part = _mm_hadd_ps (denom_part, denom_part);
            __m128 opr = _mm_rcp_ps(_mm_fmadd_ps(denom_part, inv_intensity, inv_intensity));

            __m128 new_alpha = _mm_sub_ps (ext_255, opr);
            __m128 new_color = _mm_mul_ps (_mm_set_ps (light.color.r, light.color.g, light.color.b, 0.f), opr);

            // constructing final color.
            __m128 resn = _mm_sub_ps (ext_255, new_alpha);
            __m128 res = _mm_sub_ps (ext_255, curr_alpha);
            __m128 q = _mm_rcp_ps (_mm_add_ps (res, resn));

            curr_color = _mm_mul_ps (_mm_fmadd_ps (res, curr_color, _mm_mul_ps (resn, new_color)), q);
            curr_alpha = _mm_min_ps (curr_alpha, new_alpha);

            break;
          }
          iter = iter->next;
        }
      }

      __m128 actual_color = _mm_min_ps(curr_color, ext_255);
      _mm_storeu_si32 (buffer_iter, _mm_shuffle_epi8 (
        _mm_cvtps_epi32 (_mm_move_ss(actual_color, curr_alpha)), 
        mm_opr_mask
      ));

      buffer_iter++;
      pos += advance_x;
    }

    pos = advance_y;
    advance_y += aux_advance_y;
  }
  
  for (uint32_t j = 0; j < new_segments.size(); j++) {
    if (new_segments[j].obj != nullptr) {
      darena.complete_free_mo(new_segments[j].obj);
    }
  }

  return *this;
}
