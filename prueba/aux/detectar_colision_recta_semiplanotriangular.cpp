
static inline int meeting_condition_for_ordering_by_point (
  const SecondLevelElement& line_1, 
  const SecondLevelElement& line_2, 
  const Dir2& position
) {
  const Dir2 vB = line_1.point1 - position;
  const Dir2 vC = line_1.point2 - position;
  const Dir2 DA = line_2.point1 - position;
  const Dir2 EA = line_2.point2 - position;
  const Dir2 Dn = Dir2 (vC.pLd(DA, vB), vB.pLd(DA, vC));
  const Dir2 En = Dir2 (vC.pLd(EA, vB), vB.pLd(EA, vC));
  const Dir2 vn = Dn - En;

  const __m128 vals = _mm_div_ps ((-En).v, vn.v);
  const __m128 cons1 = _mm_cmpgt_ps(vn.v, _mm_set1_ps(0.0001f));
  const __m128 cons2 = _mm_cmplt_ps(vn.v, _mm_set1_ps(-0.0001f));
  const __m128 vals1 = _mm_and_ps(cons1, vals);
  const __m128 vals2 = _mm_or_ps(_mm_and_ps(cons2, vals), _mm_and_ps(cons1, _mm_set1_ps(1.f)));
  const __m128 cI = _mm_max_ps(_mm_shuffle_ps (vals1, vals1, 0b01), vals1);
  const __m128 cS = _mm_min_ps(_mm_shuffle_ps (vals2, vals2, 0b01), vals2);
  const __m128 coef = _mm_mul_ps(_mm_set1_ps(0.5f), _mm_add_ps(cI, cS));

  const Dir2 middle_kiss = Dir2(_mm_fmadd_ps(vn.v, coef, En.v));
  int cond_one = _mm_movemask_ps(_mm_cmpgt_ps (middle_kiss.v, _mm_set1_ps(0.0001f))) & 0b11;
  cond_one &= cond_one >> 1;

  int cond_two = middle_kiss.x + middle_kiss.y > 1.f;

  return cond_one | (cond_two << 1);
}
