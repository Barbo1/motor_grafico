#include "../../../../headers/concepts/image_modifier.hpp"
#include "../utils/circle_bound_maker.cpp"

ImageModifier ImageModifier::circle (int radio, SDL_Color color) {
  int* bounds = new int[4*radio - 4];
  circle_bound_maker (radio, bounds, 0);

  ImageModifier ret = ImageModifier::bounder(bounds, 2*radio, 2*radio, color);
  return ret;
}
