#include "../../headers/primitives/arena.hpp"
#include <iostream>

int main () {
  Arena arena = Arena(20);
  std::byte* a = arena.alloc(10);
  std::cout << a << std::endl;

  std::byte* e = arena.alloc(5, 3);
  if (e != nullptr) {
    std::cout << "error" << std::endl;
    std::exit(-1);
  }

  std::byte* b = arena.alloc(4, 3);
  std::cout << b << std::endl;

  std::cout << "bien!" << std::endl;

  return 0;
}
