#include "../../../../headers/concepts/gui.hpp"

Button::Button (
  std::function<void(Dir2)> selected_fn,
  std::function<void(Dir2)> observed_fn,
  std::function<void(Dir2)> quiet_fn,
  Dir2 position,
  Dir2 dims
) noexcept : selected_fn(selected_fn),
   observed_fn(observed_fn),
   quiet_fn(quiet_fn),
   position(position),
   dims(dims) {}
