#include "../../../../headers/concepts/gui.hpp"

Slider::Slider (
  std::function<void(Dir2)> base_fn,
  std::function<void(Dir2)> sign_fn,
  Dir2 position,
  Dir2 base_dims,
  Dir2 sign_dims,
  uint32_t number_values,
  SliderDirection direction
) noexcept: base_fn(base_fn), 
   sign_fn(sign_fn), 
   position(position), 
   base_dims(base_dims), 
   sign_dims(sign_dims), 
   max_index(static_cast<float>(number_values)),
   curr_index(0),
   direction(direction) {}
