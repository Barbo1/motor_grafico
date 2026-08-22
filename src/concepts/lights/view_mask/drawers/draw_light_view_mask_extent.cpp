#include "../../../../../headers/concepts/lights.hpp"
#include "../../../../../headers/primitives/math.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <emmintrin.h>
#include <utility>

ViewMask& ViewMask::draw_light_view_mask (
  Arena& arena,
  DynamicalArena& darena,
  const std::span<std::pair<Light, MaskObjectList>, std::dynamic_extent>& segments,
  const Dir2& screen_dims 
) {
  Uint32* buffer_first = (Uint32*)img->pixels;
  for (uint32_t i = 0; i < (uint32_t)(img->w * img->h); i++)
    buffer_first[i] = 255;

  for (uint32_t ss = 0; ss < segments.size(); ss++) {
    const Light& light = segments[ss].first;
    const MaskObjectList& parts = segments[ss].second;

    Dir2 light_pos = Dir2(light.position);
    Dir2 bound_min;
    uint32_t xmin, ymin;
    int32_t width, height;
    {
      __m128 rebound;
      switch (light.type) {
        case LightType::LT_CENTERD:
          rebound = find_light_screen_bounding(light, screen_dims);
          break;
        case LightType::LT_FOCALIZED:
          rebound = find_focal_screen_bounding(light, light.focal_line[0], light.focal_line[1], screen_dims);
          break;
        default:
          std::unreachable();
          break;
      }
      __m128i reboundi = _mm_cvtps_epi32(rebound);

      bound_min = Dir2::from_well(_mm_permute_ps(rebound, 0b11101110));
      xmin = _mm_extract_epi32(reboundi, 2);
      ymin = _mm_extract_epi32(reboundi, 3);
      uint32_t xmax = _mm_extract_epi32(reboundi, 0);
      uint32_t ymax = _mm_extract_epi32(reboundi, 1);
      height = ymax - ymin;

      // align to 4 to be able to iterate over __m128 array.
      xmax = (xmax >> 2) << 2;
      xmin = (xmin >> 2) << 2;
      width = xmax - xmin;
    }
  
    if (width <= 0 || height <= 0) 
      continue;

    ArenaConstexFlag context = arena.get_context();
    Uint32* another_buffer = arena.atalloc<Uint32>(width * height);
    if (another_buffer == nullptr) 
      continue;

    {
      // filter and reposition lines.
      MaskObjectList filtered = filter_lines_point_view(darena, parts, light, screen_dims);
      light_pos -= bound_min;
      for (auto iter = filtered.obj; iter != nullptr; iter = iter->next) {
        iter->point1.store(Dir2(iter->point1) - bound_min);
        iter->point2.store(Dir2(iter->point2) - bound_min);
      }

      // set initial conditions based on light type.
      Uint32* construct_another_buffer = another_buffer;
      switch (light.type) {
        case LightType::LT_CENTERD:
          for (int32_t k = 0; k < width * height; k++)
            construct_another_buffer[k] = 0;
          break;
        case LightType::LT_FOCALIZED: {
            for (int32_t k = 0; k < width * height; k++)
              construct_another_buffer[k] = 255;
    
            MaskObject* obj = darena.alloc_mo();
            obj->point1.store(light.focal_line[0] - bound_min);
            obj->point2.store(light.focal_line[1] - bound_min);
            obj->next = nullptr;
            MaskObjectList first_ofuscation = {.obj = obj, .size = 1};
            fill_view_with_shadows (
              construct_another_buffer, 
              width, 
              height, 
              darena,
              first_ofuscation,
              light_pos,
              0
            );
            darena.free_mo(obj);
          }
          break;

        default:
          std::unreachable();
          break;
      }

      // filling image.
      fill_view_with_shadows (construct_another_buffer, width, height, darena, filtered, light_pos, 255);
      fill_remain_with_lights_4 (construct_another_buffer, width, height, light_pos, light);
      darena.complete_free_mo (filtered.obj);
    }

    {
      // appending both images.
      const __m128 ext_255 = _mm_set1_ps (255.f);
      const __m128i mm_opr_mask = _mm_set_epi8 (0,0,0,0,0,0,0,0,0,0,0,0,12,8,4,0);
      const uint32_t advance_y = img->w - width;

      Uint32* buffer_this = ((Uint32*)img->pixels) + xmin + ymin * img->w;

      for (int32_t h = 0; h < height; h++) {
        for (int32_t w = 0; w < width; w+=4) {
          /* first iteration. */
          __m128i charged_t = _mm_loadu_si128 ((__m128i*)buffer_this);
          __m128i charged_m = _mm_loadu_si128 ((__m128i*)another_buffer);
          __m128 ready = _mm_undefined_ps();

          for (uint32_t j = 0; j < 4; j++) {
            __m128 oprt = _mm_cvtepi32_ps (_mm_cvtepu8_epi32 (charged_t));
            __m128 oprm = _mm_cvtepi32_ps (_mm_cvtepu8_epi32 (charged_m));

            // calculating q.
            __m128 rest = _mm_sub_ss(ext_255, oprt);
            __m128 resm = _mm_sub_ss(ext_255, oprm);
            __m128 opq = _mm_rcp_ss (_mm_add_ss (rest, resm));
            __m128 opq_m = _mm_mul_ss (resm, opq);
            __m128 opq_t = _mm_mul_ss (rest, opq);

            // constructing pixel.
            __m128 pixel_color = _mm_fmadd_ps (
              oprt, 
              _mm_shuffle_ps (opq_t, opq_t, 0), 
              _mm_mul_ps (oprm, _mm_shuffle_ps (opq_m, opq_m, 0))
            );
            __m128 pixel_alpha = _mm_min_ss (oprt, oprm);
            __m128 pixel = _mm_castsi128_ps(
              _mm_shuffle_epi8 (
                _mm_cvtps_epi32 (_mm_move_ss (pixel_color, pixel_alpha)),
                 mm_opr_mask
              )
            );

            // putting pixel in ready vector.
            ready = _mm_move_ss(ready, pixel);
            ready = _mm_shuffle_ps(ready, ready, 0b10010011);
            charged_m = _mm_shuffle_epi32(charged_m, 0b10010011);
            charged_t = _mm_shuffle_epi32(charged_t, 0b10010011);
          }

          _mm_storeu_si128 ((__m128i*)buffer_this, _mm_castps_si128(ready));

          buffer_this+=4;
          another_buffer+=4;
        }

        buffer_this += advance_y;
      }
    }
    arena.go_back_context(context);
  }

  return *this;
}
