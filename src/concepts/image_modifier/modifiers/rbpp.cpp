#include "../../../../headers/concepts/image_modifier.hpp"
#include <cstdint>

ImageModifier& ImageModifier::rbpp (uint32_t number_reduction) {
  Uint32* arr = (Uint32*)this->texture->pixels;
  Uint32 coef = ~((0x01010100 << number_reduction) - 0x01010100);
  __m128i* arr_128 = (__m128i*)this->texture->pixels;
  __m128i _mcoef = _mm_set1_epi32 (coef);
  uint32_t top = this->texture->h * this->texture->w;

  for (uint32_t i = 0; i < (top >> 2); i++, arr_128++)
    *arr_128 = _mm_and_si128(*arr_128, _mcoef);
  for (uint32_t i = top & ~3; i < top; i++)
    arr[i] &= coef;
  return *this;
}
