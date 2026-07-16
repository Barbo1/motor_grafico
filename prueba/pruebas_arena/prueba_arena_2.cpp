#include "../../headers/primitives/arena.hpp"
#include <iostream>

int main () {
  Arena arena = Arena(20);
  std::byte* a = arena.alloc(10);
  std::cout << a << std::endl;

  std::byte* b = arena.alloc(21);
  if (b != nullptr) {
    std::cout << "error" << std::endl;
    std::exit(-1);
  }

  std::byte* c = arena.alloc(2);
  std::cout << c << std::endl;

  std::byte* d = arena.alloc(8);
  std::cout << d << std::endl;

  std::byte* e = arena.alloc(1);
  if (e != nullptr) {
    std::cout << "error" << std::endl;
    std::exit(-1);
  }

  std::cout << "bien!" << std::endl;

  return 0;
}
