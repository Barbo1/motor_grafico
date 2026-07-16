#include "../../headers/primitives/arena.hpp"
#include <iostream>

int main () {
  Arena arena = Arena(20000);
  std::byte* a = arena.alloc(10);
  std::cout << a << std::endl;
  std::byte* b = arena.alloc(21, 4);
  std::cout << b << std::endl;
  std::byte* c = arena.alloc(2);
  std::cout << c << std::endl;
  arena.restart();
  std::byte* d = arena.alloc(7);
  std::cout << d << std::endl;
  std::byte* e = arena.alloc(7);
  std::cout << e << std::endl;
  return 0;
}
