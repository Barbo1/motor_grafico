#include "../../../../../../headers/sp_objects/impulse.hpp"

template <ImpulseType IT, UnitType UT, Function F>
float Impulse<IT, UT, F>::get_radio() {
  return this->radio;
}

template float Impulse<IT_HOLE, UT_NONE, FT_CONSTANT>::get_radio();
