#include "../../../../../headers/concepts/lights.hpp"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <emmintrin.h>

ViewMask& ViewMask::draw_light_view_mask (const Light& light, const std::vector<MaskObject>& segments, const Uint32 shadow_color) {
  fill_view_with_shadows (this->img, light.position, segments, shadow_color);
  Uint32* buffer = (Uint32*)img->pixels;

  float add_coef = light.attenuation / std::sqrt(light.intensity);
  Dir2 pos,
    init_pos = -light.position * add_coef, 
    inv_intensity = Dir2 {1.f / light.intensity, 0.f},
    add_coef_x = Dir2 {_mm_set_ps (0.f, 0.f, 0.f, add_coef)},
    add_coef_y = Dir2 {_mm_set_ps (0.f, 0.f, add_coef, 0.f)};
  __m128 mm_color = _mm_set_ps (light.color.r, light.color.g, light.color.b, 0.f);
  __m128 mm_alpha = _mm_set_ps (0.f, 0.f, 0.f, 255.f);

  pos = init_pos;
  for (uint32_t i = 0; i < (uint32_t)img->h; i++) {
    for (uint32_t j = 0; j < (uint32_t)img->w; j++) {
      if (*buffer == 0) {
        __m128 opr = _mm_add_ps(_mm_mul_ps(pos.v, pos.v), inv_intensity.v);
        float light_coef = _mm_cvtss_f32(_mm_rcp_ps(_mm_hadd_ps(opr, opr)));
        opr = _mm_set1_ps (light_coef);

        __m128i opri = _mm_cvtps_epi32 (_mm_blend_ps (
          _mm_mul_ps (mm_color, opr), 
          _mm_sub_ps (mm_alpha, opr), 
          0b0001
        ));
        opri = _mm_packs_epi32(opri, opri);
        opri = _mm_packs_epi16(opri, opri);
        _mm_storeu_si32(buffer, opri);
      }
      buffer++;
      pos += add_coef_x;
    }
    init_pos += add_coef_y;
    pos = init_pos;
  }

  return *this;
}
