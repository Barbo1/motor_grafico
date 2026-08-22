#include "../../../../../headers/concepts/lights.hpp"
#include "../../../../../headers/primitives/math.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <emmintrin.h>
#include <smmintrin.h>
#include <utility>

ViewMask& ViewMask::draw_light_mask (
  Arena& arena,
  DynamicalArena& darena,
  const std::span<std::pair<Light, MaskObjectList>, std::dynamic_extent>& segments,
  const Dir2& screen_dims 
) {
  // initialy fill the image with shadows.
  Uint32* buffer_first = (Uint32*)img->pixels;
  for (uint32_t i = 0; i < (uint32_t)(img->w * img->h); i++)
    buffer_first[i] = 255;

  for (uint32_t ss = 0; ss < segments.size(); ss++) {
    const Light& light = segments[ss].first;
    const MaskObjectList& parts = segments[ss].second;

    Dir2 light_pos = Dir2(light.position);
    Dir2 bound_min;
    int32_t width, height, xmin, ymin;
    {
      // calculating bound of the imaged of the light.
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
      uint32_t xmax = _mm_extract_epi32(reboundi, 0);
      uint32_t ymax = _mm_extract_epi32(reboundi, 1);
      uint32_t xmin_prev = _mm_extract_epi32(reboundi, 2);
      ymin = _mm_extract_epi32(reboundi, 3);
      
      // xmin and xmax must be aligned to 4 bytes to be able to iterate over __m128 array.
      xmin = xmin_prev & ~3;
      xmax = xmax & ~3;
      height = ymax - ymin;
      width = xmax - xmin;

      // reposition bounds to fit the xmin initial position.
      uint32_t diff = xmin_prev - xmin;
      bound_min = Dir2::from_well(_mm_permute_ps(rebound, 0b11101110)) - Dir2(diff, 0.f);
    }
  
    if (width <= 0 || height <= 0) 
      continue;

    ArenaConstexFlag context = arena.get_context();
    Uint32* another_buffer = arena.atalloc<Uint32>(width * height);
    if (another_buffer == nullptr) 
      continue;

    {
      // filter and reposition lines.
      light_pos -= bound_min;
      MaskObjectList filtered = filter_lines_point_view(darena, parts, light, screen_dims);
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
      const __m128i alpha_mask = _mm_set_epi8 (0,0,0,0xFF,0,0,0,0xFF,0,0,0,0xFF,0,0,0,0xFF);
      const uint32_t advance_y = img->w - width;

      Uint32* buffer_this = ((Uint32*)img->pixels) + xmin + ymin * img->w;

      for (int32_t h = 0; h < height; h++) {
        for (int32_t w = 0; w < width; w+=4) {
          __m128i charged_t = _mm_loadu_si128 ((__m128i*)buffer_this);
          __m128i charged_m = _mm_loadu_si128 ((__m128i*)another_buffer);
          __m128i pixel_2, pixel_4, final_alphas;
          __m128 mult_coef;

          {
            // calculating final alphas.
            __m128i alphasm_i = _mm_blendv_epi8 (_mm_setzero_si128(), charged_m, alpha_mask);
            __m128i alphast_i = _mm_blendv_epi8 (_mm_setzero_si128(), charged_t, alpha_mask);
            final_alphas = _mm_min_epi32(alphast_i, alphasm_i);

            // calculating multiplicative coeficients for colors.
            __m128 alphasm = _mm_sub_ps (ext_255, _mm_cvtepi32_ps (alphasm_i));
            __m128 alphast = _mm_sub_ps (ext_255, _mm_cvtepi32_ps (alphast_i));
            mult_coef = _mm_div_ps (alphasm, _mm_add_ps (alphast, alphasm));
          }

          {
            // first pixel.
            __m128 oprt1 = _mm_cvtepi32_ps (_mm_cvtepu8_epi32 (charged_t));
            __m128 oprm1 = _mm_cvtepi32_ps (_mm_cvtepu8_epi32 (charged_m));
            __m128 color_diff1 = _mm_sub_ps (oprm1, oprt1);
            __m128 pixel_1 = _mm_fmadd_ps (_mm_permute_ps(mult_coef, 0), color_diff1, oprt1);

            // second pixel.
            __m128 oprt2 = _mm_cvtepi32_ps (_mm_cvtepu8_epi32 (_mm_shuffle_epi32(charged_t, 0b01)));
            __m128 oprm2 = _mm_cvtepi32_ps (_mm_cvtepu8_epi32 (_mm_shuffle_epi32(charged_m, 0b01)));
            __m128 color_diff2 = _mm_sub_ps (oprm2, oprt2);
            __m128 pixel_color2 = _mm_fmadd_ps (_mm_permute_ps(mult_coef, 0b01010101), color_diff2, oprt2);
            pixel_2 = _mm_packus_epi32 (
              _mm_cvtps_epi32 (pixel_1),
              _mm_cvtps_epi32 (pixel_color2)
            );
          }

          {
            // third pixel.
            __m128 oprt3 = _mm_cvtepi32_ps (_mm_cvtepu8_epi32 (_mm_shuffle_epi32(charged_t, 0b10)));
            __m128 oprm3 = _mm_cvtepi32_ps (_mm_cvtepu8_epi32 (_mm_shuffle_epi32(charged_m, 0b10)));
            __m128 color_diff3 = _mm_sub_ps (oprm3, oprt3);
            __m128 pixel_3 = _mm_fmadd_ps (_mm_permute_ps(mult_coef, 0b10101010), color_diff3, oprt3);

            // forth pixel.
            __m128 oprt4 = _mm_cvtepi32_ps (_mm_cvtepu8_epi32 (_mm_shuffle_epi32(charged_t, 0b11)));
            __m128 oprm4 = _mm_cvtepi32_ps (_mm_cvtepu8_epi32 (_mm_shuffle_epi32(charged_m, 0b11)));
            __m128 color_diff4 = _mm_sub_ps (oprm4, oprt4);
            __m128 pixel_color4 = _mm_fmadd_ps (_mm_permute_ps(mult_coef, 0b11111111), color_diff4, oprt4);
            pixel_4 = _mm_packus_epi32 (
              _mm_cvtps_epi32 (pixel_3),
              _mm_cvtps_epi32 (pixel_color4)
            );
          }

          // end loop.
          _mm_storeu_si128 ((__m128i*)buffer_this, _mm_blendv_epi8(
            _mm_packus_epi16(pixel_2, pixel_4), 
            final_alphas, 
            alpha_mask
          ));

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
