#pragma once

static float fisqrt (float x) {
  union {int xi; float xf;};

  xf = x;
  float xhalf = 0.5 * xf;
  xi = 0x5f375a82 - (xi >> 1);
  return xf*(1.5f - xhalf * xf * xf);
}
