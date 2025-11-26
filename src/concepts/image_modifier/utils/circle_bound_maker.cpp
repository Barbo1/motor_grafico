#include "../../../../headers/concepts/image_modifier.hpp"

void ImageModifier::circle_bound_maker (int radio, int* bounds, int offset) {
  int* biter = bounds;
  const float radio_2 = radio * radio;

  float y = radio-1;
  int x = std::sqrt(radio + y);
  for (; y > 0; y-=1, x = std::sqrt(radio_2 - y*y)) {
    *(biter++) = radio-x;
    *(biter++) = radio+x;
  }
  for (int* rbiter = biter-1; rbiter >= bounds; rbiter-=2) {
    *(biter++) = *(rbiter-1);
    *(biter++) = *rbiter;
  }
  biter = bounds;
  for (int i = 0, sum = offset + 2 * (radio + offset) * offset; i < 2 * radio - 2; i++, sum += 2 * (radio + offset)) {
    *(biter++) += sum;
    *(biter++) += sum;
  }
}
