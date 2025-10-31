#include "../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, AngType AT, Function F>
float Impulse<IT, AT, F>::get_radio() {
  return this->radio;
}

template float Impulse<IT_HOLE, AT_NONE, FT_CONSTANT>::get_radio();
