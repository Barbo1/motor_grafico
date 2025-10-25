#include "../../../headers/primitives/operations.hpp"
#include <cstdint>
#include <bit>

float bound (const float& f, const float& b) {
  uint32_t fi = std::bit_cast<uint32_t>(f);
  asm (
    "MOV %%eax, %%edx\n\t"
    "MOV %%eax, %%ecx\n\t"
    "AND $0x7FFFFFFF, %%edx\n\t"
    "SUB %%ebx, %%edx\n\t"
    "SAR $31, %%edx\n\t"
    "AND %%edx, %%ecx\n\t"
    "ANDN %%ebx, %%edx, %%edx\n\t"
    "OR %%ecx, %%edx\n\t"
    "AND $0x80000000, %%eax\n\t"
    "OR %%edx, %%eax\n\t"
    : "+a"(fi)
    : "b"(b)
    : "ecx", "edx"
  );
  return std::bit_cast<float>(fi);
}
