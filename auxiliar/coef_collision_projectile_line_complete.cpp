float coef_collision_projectil_line (Dir2 C, Dir2 v, float R, Dir2 A, Dir2 u) {
  __m128 b = _mm_sub_ps(C.v, A.v);
  __m128 R_ext = _mm_set1_ps(R);
  __m128 R2 = _mm_mul_ps(R_ext, R_ext);

  __m128 inv_norms = _mm_movelh_ps(v.v, u.v);
  inv_norms = _mm_mul_ps(inv_norms, inv_norms);
  inv_norms = _mm_hadd_ps(inv_norms, _mm_undefined_ps());
  inv_norms = _mm_rsqrt_ps(inv_norms);
  __m128 u_inv_norm = _mm_permute_ps(inv_norms, 0b01010101);
  __m128 v_inv_norm = _mm_permute_ps(inv_norms, 0b00000000);
  __m128 u_n = _mm_mul_ps(u.v, u_inv_norm);

  __m128 c_p = _mm_mul_ps(b, u_inv_norm);
  c_p = _mm_mul_ps(c_p, u_n);
  c_p = _mm_max_ps(_mm_min_ps(_mm_hadd_ps(c_p, c_p), _mm_set1_ps(1.f)), _mm_setzero_ps());
  c_p = _mm_fnmadd_ps(u.v, c_p, b);

  __m128 c_p_norm = _mm_mul_ps(c_p, c_p);
  c_p_norm = _mm_hadd_ps(c_p_norm, c_p_norm);

  __m128 cond_1 = _mm_cmplt_ps(c_p_norm, R2);
  if (_mm_movemask_ps(cond_1))
    return 0.f;

  __m128 v_n = _mm_mul_ps(v.v, v_inv_norm);
  __m128 bLs = _mm_xor_ps(_mm_permute_ps(b, 0b00010001), _mm_set_ps(0.f, -0.f, 0.f, -0.f));
  __m128 vLn = _mm_permute_ps(v_n, 0b00010001);
  __m128 p = _mm_mul_ps(bLs, u_n);
  p = _mm_hadd_ps(p, p);

  __m128 coef_1 = _mm_div_ps(_mm_mul_ps(R_ext, p), _mm_max_ps(R_ext, _mm_andnot_ps(_mm_set1_ps(-0.f), p)));
  __m128 vec_1 = _mm_fnmadd_ps(u_n, coef_1, bLs);
  __m128 coef_2 = _mm_mul_ps(vLn, u.v);
  coef_2 = _mm_hsub_ps(coef_2, coef_2);
  __m128 vec_2 = _mm_div_ps(v_n, coef_2);
  __m128 coef_f = _mm_mul_ps(vec_1, vec_2);
  coef_f = _mm_max_ps(_mm_min_ps(_mm_hadd_ps(coef_f, coef_f), _mm_set1_ps(1.f)), _mm_setzero_ps());
  __m128 f = _mm_fmsub_ps(u.v, coef_f, b);

  __m128 sqrt_part_1 = _mm_mul_ps(vLn, f);
  __m128 ind_term = _mm_mul_ps(v_n, f);
  sqrt_part_1 = _mm_hsub_ps(sqrt_part_1, sqrt_part_1);
  ind_term = _mm_hadd_ps(ind_term, ind_term);
  sqrt_part_1 = _mm_mul_ps(sqrt_part_1, sqrt_part_1);

  __m128 sqrt_part = _mm_sqrt_ps(_mm_sub_ps(R2, sqrt_part_1));
  __m128 d = _mm_mul_ps(_mm_sub_ps(ind_term, sqrt_part), v_inv_norm);
  
  __m128 cond_2 = _mm_and_ps(_mm_cmplt_ps(d, _mm_set1_ps(1.f)), _mm_cmpgt_ps(d, _mm_set1_ps(0.f)));
  __m128 cond_3 = _mm_cmplt_ps(sqrt_part_1, R2);

  __m128 cond_long = _mm_and_ps(cond_2, cond_3);

  if (_mm_movemask_ps(cond_long))
    return _mm_cvtss_f32(d);
  else
    return INFINITY;
}
