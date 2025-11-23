#pragma once

/* Sign function. */
float sgn (float num);

/* Functions that return the maximum or minimum. */
float min0 (float num);
float min (const float& num1, const float& num2);
float max (const float& num1, const float& num2);

void max_min (const float& num1, const float& num2, float* max, float* min);
void max_min (
  const float& num1, const float& num2, 
  const float& res1, const float& res2, 
  float* max, float* min
);

/* Comparison functions. */
int gti (const float& x, const float& y);
int gei (const float& x, const float& y);
