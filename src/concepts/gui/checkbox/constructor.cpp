#include "../../../../headers/concepts/gui.hpp"

CheckBox::CheckBox (
  std::function<void(Dir2)> active_fn,
  std::function<void(Dir2)> deactive_fn,
  Dir2 position,
  Dir2 dims,
  bool active
) noexcept: active_fn(active_fn), 
   deactive_fn(deactive_fn), 
   position(position), 
   dims(dims), 
   active(active) {}
