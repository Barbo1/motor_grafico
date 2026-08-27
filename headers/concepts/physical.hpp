#pragma once 

#include "../primitives/vectors.hpp"
#include "./global.hpp"

#include <type_traits>
#include <concepts>

#define DRAW_RATE 5.0f
#define MOVEMENT_COEFITIENT 100.f
#define ENERGY_DISIPATION 1.0f

template <typename T>
concept Physical = requires(T elem, Global* glb, AngDir2 a) {
  {elem.position} -> std::same_as<MemDir2>;
  {elem.velocity} -> std::same_as<MemDir2>;
  {elem.force} -> std::same_as<MemDir2>;
  {elem.collision_normal} -> std::same_as<MemDir2>;
  requires (
    std::is_same_v<decltype(elem.mass), float>
  ) || (
    std::is_same_v<decltype(elem.density), float> && 
    std::is_same_v<decltype(elem.area), float> && 
    std::is_function_v<decltype(elem.get_mass)> && std::is_same_v<std::result_of<decltype(elem.get_mass)()>, float>
  );
};
