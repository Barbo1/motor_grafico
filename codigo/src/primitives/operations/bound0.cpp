#include "../../../headers/primitives/operations.hpp"
#include <cstdint>
#include <bit>

float bound0 (const float& f, const float& b) {
  uint32_t fi = std::bit_cast<uint32_t>(f);
  asm (
    "MOV %%eax, %%edx\n\t"
    "MOV %%eax, %%ecx\n\t"
    "SAR $31, %%eax\n\t"
    "SUB %%ebx, %%ecx\n\t"
    "SAR $31, %%ecx\n\t"
    "AND %%ecx, %%edx\n\t"
    "ANDN %%ebx, %%ecx, %%ecx\n\t"
    "OR %%edx, %%ecx\n\t"
    "ANDN %%ecx, %%eax, %%eax\n\t"
    : "+a"(fi)
    : "b"(b)
    : "ecx", "edx"
  );
  return std::bit_cast<float>(fi);
}
