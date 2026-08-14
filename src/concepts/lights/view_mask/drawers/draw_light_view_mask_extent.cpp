#include "../../../../../headers/concepts/lights.hpp"
#include "../../../../../headers/primitives/math.hpp"
#include <SDL2/SDL_render.h>
#include <cstdint>

ViewMask& ViewMask::draw_light_view_mask (
  //Arena& arena,
  DynamicalArena& darena,
  const std::span<std::pair<Light, MaskObjectList>, std::dynamic_extent>& segments,
  const Dir2& screen_dims 
) {
  __m128i* buffer_iter_128 = (__m128i*)img->pixels;
  __m128i ext_255_i = _mm_set1_epi32(255);
  for (uint32_t i = 0; i < (uint32_t)(img->w * img->h) / 4; i++) {
    _mm_store_si128(buffer_iter_128, ext_255_i);
    buffer_iter_128++;
  }

  for (uint32_t j = 0; j < segments.size(); j++) {
    const Light& light = segments[j].first;
    const MaskObjectList& segment = segments[j].second;
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
      float coef = light.attenuation / std::sqrt(light.intensity);

      for (MaskObject* iter = viewed.obj; iter != nullptr; iter = iter->next) {
        iter->point1.store((Dir2(iter->point1) - light_pos) * coef);
        iter->point2.store((Dir2(iter->point2) - light_pos) * coef);
      }

      Dir2 K, dimsK;
      Dir2 bound_max, bound_min;
      uint32_t xmax, xmin, ymax, ymin;
      {
        float light_bound = std::sqrt((light.intensity - 1.f) / (light.attenuation * light.attenuation));
        Dir2 light_dims = Dir2(light_bound, light_bound);
        auto [K_aux, dimsK_aux] = calculate_light_bound (screen_dims, light_pos, light_dims);
        K = K_aux;
        dimsK = dimsK_aux;
        
        {
          Dir2 bound_1 = K - dimsK;
          Dir2 bound_2 = K + dimsK;
          
          bound_max = Dir2::from_well(_mm_min_ps(_mm_max_ps(bound_1.v, bound_2.v), screen_dims.v));
          bound_min = Dir2::from_well(_mm_min_ps(_mm_min_ps(bound_1.v, bound_2.v), screen_dims.v));
          __m128i max = _mm_cvtps_epi32(bound_max.v);
          __m128i min = _mm_cvtps_epi32(bound_min.v);
          xmax = _mm_extract_epi32(max, 0);
          xmin = _mm_extract_epi32(min, 0);
          ymax = _mm_extract_epi32(max, 1);
          ymin = _mm_extract_epi32(min, 1);
        }
      }

      uint32_t y_advance_pos = img->w - (xmax - xmin);
      Dir2 init_pos = bound_min * coef;
      Dir2 pos = init_pos; 
      Dir2 add_coef_x = Dir2::from_well (_mm_set_ps (0.f, coef, 0.f, coef));
      Dir2 add_coef_y = Dir2::from_well (_mm_set_ps (coef, 0.f, coef, 0.f));

      __m128 color = _mm_set_ps (light.color.r, light.color.g, light.color.b, 0.f);
      __m128 inv_intensity = _mm_rcp_ps (_mm_set1_ps (light.intensity));
      __m128 ext_255 = _mm_set1_ps(255.f);
      __m128i mm_opr_mask = _mm_set_epi8 (0,0,0,0,0,0,0,0,0,0,0,0,12,8,4,0);

      light_pos *= coef;

      Uint32* buffer_iter = (Uint32*)img->pixels + (ymin * img->w) + xmin;
      for (uint32_t h = ymin; h < ymax; h++) {
        for (uint32_t w = xmin; w < xmax; w++) {

          MaskObject* iter = viewed.obj;
          //MaskObject* prev = nullptr;
          for (uint32_t k = 0; k < viewed.size; k++) {
            if (test_collision_triangle_point (light_pos, Dir2(iter->point1), Dir2(iter->point2), pos)) {
              uint32_t num = *buffer_iter;

              __m128 curr = _mm_cvtepi32_ps (_mm_cvtepu8_epi32 (_mm_set1_epi32(num)));
              __m128 curr_color = curr;
              __m128 curr_alpha = _mm_permute_ps(curr, 0);
                
              __m128 denom_part = _mm_sub_ps (pos.v, light_pos.v);
              denom_part = _mm_mul_ps (denom_part, denom_part);
              denom_part = _mm_hadd_ps (denom_part, denom_part);
              __m128 opr = _mm_rcp_ps(_mm_add_ps(denom_part, inv_intensity));

              __m128 new_alpha = _mm_sub_ps (ext_255, opr);
              __m128 new_color = _mm_mul_ps (color, opr);

              // constructing final color.
              __m128 resn = _mm_sub_ps (ext_255, new_alpha);
              __m128 res = _mm_sub_ps (ext_255, curr_alpha);
              __m128 q = _mm_rcp_ps (_mm_add_ps (res, resn));

              curr_color = _mm_mul_ps (_mm_fmadd_ps (res, curr_color, _mm_mul_ps (resn, new_color)), q);
              curr_alpha = _mm_min_ps (curr_alpha, new_alpha);

              __m128 actual_color = _mm_min_ps(curr_color, ext_255);
              _mm_storeu_si32 (buffer_iter, _mm_shuffle_epi8 (
                _mm_cvtps_epi32 (_mm_move_ss(actual_color, curr_alpha)), 
                mm_opr_mask
              ));

              /*
              if (prev != nullptr && prev != viewed.obj) {
                MaskObject* change = viewed.obj;
                MaskObject* change_next = change->next;
                prev->next = change;
                change->next = iter->next;
                iter->next = change_next;
                viewed.obj = iter;
              }*/

              break;
            }
            //prev = iter;
            iter = iter->next;
          }

          buffer_iter++;
          pos += add_coef_x;
        }

        buffer_iter += y_advance_pos;
        init_pos += add_coef_y;
        pos = init_pos;
      }

      darena.complete_free_mo(viewed.obj);
    }
  }
  
  return *this;
}
