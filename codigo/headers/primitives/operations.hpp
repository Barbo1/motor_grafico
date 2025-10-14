#pragma once

/* Sign function. */
float sgn (float num);

/* Functions that return the maximum or minimum. */
float max0 (float num);
float min0 (float num);
float min (const float& num1, const float& num2);
float max (const float& num1, const float& num2);

void max_min (
  const float& num1, const float& num2, 
  const float& res1, const float& res2, 
  float* max, float* min
);

/* Absolute value function. */
float absv (float num);
void absv (float* num);

/* Fast inverse square root. */
float fisqrt (float num);

/* Comparison functions. */
int gti (const float& x, const float& y);
int gei (const float& x, const float& y);

/* Bound absolute operation: for float f and a bound b, returns f if |f| < b, 
 * else sgn(f)*b. Precondition: b > 0.
 * */
float bound (const float& f, const float& b);
