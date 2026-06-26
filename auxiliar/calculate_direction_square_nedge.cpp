
/* Return the direction from the square to the segment, and if they don't collide,
 * return Dir2(0.f, 0.f).
 *
 * 'A' is the center of the square,
 * 'dims' are the dimenions of the square,
 * 'E' is the begining point of the segment,
 * 'vD' is the direction of the segment(finishing in the last point of the segment).
 * */
template<std::size_t N>
inline Dir2 calculate_direction_square_nedge (
    Dir2 C, Dir2 dims, 
    std::array<std::pair<MemDir2, MemDir2>, N> segments,
    std::array<std::pair<Dir2, Dir2>, N>& filtered, uint32_t& many
) {
  many = 0;

  __m128 sngs_1 = _mm_set_ps(-0.f,-0.f,0.f,0.f);
  __m128 ws = _mm_xor_ps(_mm_blend_ps(dims.v, _mm_setzero_ps(), 0b1010), sngs_1);
  __m128 hs = _mm_xor_ps(_mm_blend_ps(dims.v, _mm_setzero_ps(), 0b0101), sngs_1);

  __m128 dims_over = _mm_shuffle_ps(dims.v, dims.v, 0b00000101);
  __m128 sgn_dims = _mm_xor_ps(_mm_unpacklo_ps(dims.v, dims.v), _mm_set_ps(-0.f, 0.f, -0.f, 0.f));
  __m128 half = _mm_set1_ps(0.5f);

  __m128 final_cond = _mm_setzero_ps();
  for (const auto& [v, D]: segments) {
    __m128 b = _mm_sub_ps(C.v, D.v);
    __m128 m = _mm_fnmadd_ps(v.v, half, b);
    __m128 vL = _mm_shuffle_ps(v.v, v.v, 0b00010001);
    __m128 denom = _mm_rcp_ps(_mm_unpacklo_ps(v.v, v.v));

    __m128 part_1 = _mm_mul_ps(_mm_add_ps(_mm_unpacklo_ps(m, m), sgn_dims), denom);
    __m128 cond_1 = _mm_cmplt_ps(_mm_andnot_ps(_mm_set1_ps(-0.f), part_1), half);

    __m128 part_2_x = _mm_mul_ps(_mm_add_ps(b, ws), vL);
    __m128 part_2_y = _mm_mul_ps(_mm_add_ps(b, hs), vL);
    __m128 part_2 = _mm_mul_ps(_mm_hsub_ps(part_2_x, part_2_y), denom);
    __m128 cond_2 = _mm_cmplt_ps(_mm_andnot_ps(_mm_set1_ps(-0.f), part_2), dims_over);

    __m128 cond = _mm_and_ps(cond_1, cond_2);
    final_cond = _mm_or_ps(cond, final_cond);
   
    Dir2 aux = Dir2::from_well(b);
    if (_mm_movemask_ps(cond) || aux.bound(dims) == aux) {
      filtered[many] = std::pair<Dir2, Dir2>{v, D};
      many++;
    }
  }

  __m128 aux = _mm_and_ps(final_cond, sgn_dims);
  return Dir2::from_well(_mm_hadd_ps(aux, aux));
}
