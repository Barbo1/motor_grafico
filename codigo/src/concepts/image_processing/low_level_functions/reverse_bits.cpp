#pragma once
#include "../../../../headers/concepts/image_processing.hpp"

uint64_t reverse_bits (uint64_t elem, uint16_t L) {
  int dif = L-1;
  for (int lf = dif, ls = 0; ls < lf; ls++, lf--) {
    uint32_t auxs = (1 << ls);
    uint32_t auxf = (1 << lf);
    elem = (elem & ~(auxs | auxf)) | (elem & auxs) << dif | (elem & auxf) >> dif;
    dif -= 2;
  }
  return elem;
}

