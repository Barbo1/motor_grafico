#include <SDL2/SDL.h>

void xshear (SDL_Point * piter, SDL_Point * pfin, float shear) {
  while (piter < pfin) {
    int y = piter->y;
    float skew = shear * (y + 0.5);
    int skewi = floor (skew);
    while (piter < pfin && y == piter->y) {
      piter->x = skewi;
      piter++;
    }

  }
  while (piter < pfin) {
    

    piter++;
  }
}
