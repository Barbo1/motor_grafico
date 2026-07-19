#pragma once 

#include "../primitives/vectors.hpp"
#include "../primitives/global.hpp"
#include <type_traits>

#define DRAW_RATE 5.0f
#define MOVEMENT_COEFITIENT 100.f
#define ENERGY_DISIPATION 1.0f

template <typename T>
concept Physical = requires(T elem, Global* glb, const AngDir2& a) {
  requires std::is_same_v<decltype(elem.position), MemDir2>;
  requires std::is_same_v<decltype(elem.velocity), MemDir2>;
  requires std::is_same_v<decltype(elem.collision_normal), MemDir2>;
  requires std::is_same_v<decltype(elem.get_mass()), float> || (
    std::is_same_v<decltype(elem.density), float> && std::is_same_v<decltype(elem.area), float>
  );
  requires std::is_void_v<decltype(elem.calculate_movement(glb, a))>;
};
