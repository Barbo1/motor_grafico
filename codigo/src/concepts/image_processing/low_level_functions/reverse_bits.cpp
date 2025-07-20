#pragma once

#include <cstdint>

/* Function that reverses the order of bits of a uint64_t integer, only in its L firsts bits. 
 * */
uint64_t reverse_bits (uint64_t elem, uint16_t L) {
  int dif = L-1;
  for (uint64_t lf = 1 << dif, ls = 1; ls < lf; ls<<=1, lf>>=1) {
    elem = (elem & ~(ls | lf)) | (elem & ls) << dif | (elem & lf) >> dif;
    dif -= 2;
  }
  return elem;
}

