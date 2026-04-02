#include "../../../../../headers/concepts/lights.hpp"

#include <cmath>
#include <cstdint>
#include <utility>
#include <immintrin.h>

static inline int meeting_condition_for_ordering_by_direction (
  const SecondLevelElement& line_1, 
  const SecondLevelElement& line_2, 
  const Dir2& d
) {

  const Dir2 v_1 = line_1.point2 - line_1.point1;
  const Dir2 v_2 = line_2.point2 - line_2.point1;

  Dir2 lipstick_marks = Dir2 {
    d.pLd (line_1.point1 - line_2.point1, v_2),
    d.pLd (line_1.point2 - line_2.point1, v_2)
  }.bound01();

  Dir2 middle_kiss = v_2.madd (
    lipstick_marks.sum() * 0.5f, 
    line_2.point1 - line_1.point1
  );

  int ret1 = v_1.pLd(middle_kiss, d) > 0.0001f;
  float coef = d.pLd(middle_kiss, v_1);
  int ret2 = 0.0001f < coef && coef < 1.0001f;
  return (ret1 << 1) | ret2;
}

static inline int meeting_condition_for_obfuscating_by_direction (
  const SecondLevelElement& line_1, 
  const SecondLevelElement& line_2, 
  const Dir2& d, 
  Dir2& lipstick_marks
) {

  const Dir2 v_1 = line_1.point2 - line_1.point1;
  const Dir2 v_2 = line_2.point2 - line_2.point1;
  const Dir2 d_L = d.percan();

  lipstick_marks = Dir2 {
    d.pLd (line_1.point1 - line_2.point1, v_2),
    d.pLd (line_1.point2 - line_2.point1, v_2)
  }.bound01();

  Dir2 middle_kiss = v_2.madd (lipstick_marks.sum() * 0.5f, line_2.point1);

  __m128 denom = _mm_set1_ps (1.f / (v_1 * d_L));
  __m128 dist = _mm_mul_ps (
    _mm_set_ps (
      0.f,
      (middle_kiss - line_1.point1) * d_L,
      (line_2.point1 - line_1.point1) * d_L,
      (line_2.point2 - line_1.point1) * d_L
    ), denom
  );
  __m128 over_both = _mm_and_ps (
    _mm_cmplt_ps (dist, _mm_set1_ps (1.0001f)), 
    _mm_cmpgt_ps (dist, _mm_set1_ps (0.0001f))
  );

  return (_mm_movemask_ps(over_both) & 0b111);
}

static inline int meeting_condition_for_ordering_by_point (const SecondLevelElement& line_1, const SecondLevelElement& line_2, const Dir2& position) {
  const Dir2 dir_p1_u1 = line_1.point1 - position; 
  const Dir2 dir_p1_u2 = line_1.point2 - position; 
  const Dir2 dir_p1_u1_L = dir_p1_u1.percan();
  const Dir2 dir_p1_u2_L = dir_p1_u2.percan(); 
  const Dir2 dir_p2_u1 = line_2.point1 - position; 
  const Dir2 dir_p2_u2 = line_2.point2 - position; 

  const Dir2 dir_v = line_2.point1 - line_2.point2;
  Dir2 lipstick_marks = Dir2 {
    dir_p1_u1.pLd(dir_p2_u1, dir_v),
    dir_p1_u2.pLd(dir_p2_u1, dir_v)
  }.bound01();

  Dir2 middle_kiss = dir_v.madd(lipstick_marks.sum() * -0.5f, line_2.point1);

  __m128 bound_p = _mm_set1_ps(0.0001f), bound_n = _mm_set1_ps(-0.0001f);

  __m128 coef = _mm_set1_ps (1.f / (dir_p1_u2_L * dir_p1_u1));
  __m128 over_both = _mm_and_ps (
    _mm_cmpgt_ps (
      _mm_mul_ps (
        _mm_set_ps (
          0.f,
          (middle_kiss - position) * dir_p1_u2_L, 
          dir_p2_u1 * dir_p1_u2_L,
          dir_p2_u2 * dir_p1_u2_L
        ), coef
      ), bound_p
    ),
    _mm_cmple_ps (
      _mm_mul_ps (
        _mm_set_ps (
          0.f,
          (middle_kiss - position) * dir_p1_u1_L, 
          dir_p2_u1 * dir_p1_u1_L,
          dir_p2_u2 * dir_p1_u1_L
        ), coef
      ), bound_n
    )
  );

  Dir2 v12p = (line_1.point1 - line_1.point2).percan();
  __m128 aux2 = _mm_mul_ps (v12p.v, dir_p1_u2.v);
  __m128 coef_2 = _mm_rcp_ss (_mm_add_ss (_mm_shuffle_ps (aux2, aux2, 0b01010101), aux2));
  coef_2 = _mm_shuffle_ps (coef_2, coef_2, 0);
  __m128 meet_cond_2 = _mm_and_ps (
    _mm_cmpgt_ps (
      _mm_mul_ps (
        _mm_set_ps (
          0.f,
          (middle_kiss - line_1.point2) * v12p,
          (line_2.point1 - line_1.point2) * v12p,
          (line_2.point2 - line_1.point2) * v12p
        ), coef_2
      ), bound_p
    ),
    _mm_cmple_ps (
      _mm_mul_ps (
        _mm_set_ps (
          0.f,
          (middle_kiss - line_1.point2) * dir_p1_u2_L,
          (line_2.point1 - line_1.point2) * dir_p1_u2_L,
          (line_2.point2 - line_1.point2) * dir_p1_u2_L
        ), coef_2 
      ), bound_n
    )
  );

  __m128 aux1 = _mm_mul_ps ((-v12p).v, dir_p1_u1.v);
  __m128 coef_1 = _mm_rcp_ss (_mm_add_ss (_mm_shuffle_ps (aux1, aux1, 0b01010101), aux1));
  coef_1 = _mm_shuffle_ps (coef_1, coef_1, 0);
  __m128 meet_cond_1 = _mm_and_ps (
    _mm_cmpgt_ps (
      _mm_mul_ps (
        _mm_set_ps (
          0.f,
          (line_1.point1 - middle_kiss) * v12p,
          (line_1.point1 - line_2.point1) * v12p,
          (line_1.point1 - line_2.point2) * v12p
        ), coef_1
      ), bound_p
    ),
    _mm_cmple_ps (
      _mm_mul_ps (
        _mm_set_ps (
          0.f,
          (middle_kiss - line_1.point1) * dir_p1_u1_L,
          (line_2.point1 - line_1.point1) * dir_p1_u1_L,
          (line_2.point2 - line_1.point1) * dir_p1_u1_L
        ),coef_1
      ), bound_n
    )
  );

  int cond_1 = (_mm_movemask_ps(_mm_and_ps (meet_cond_1, meet_cond_2)) & 0b111) > 0;
  int cond_2 = (_mm_movemask_ps(over_both) & 0b111) > 0;

  return (cond_1 << 1) | cond_2;
}

static inline int meeting_condition_for_obfuscating_by_point (
  const SecondLevelElement& line_1, const SecondLevelElement& line_2, const Dir2& position, 
  Dir2& lipstick_marks
) {

  const Dir2 dir_p1_u1 = line_1.point1 - position; 
  const Dir2 dir_p1_u2 = line_1.point2 - position; 
  const Dir2 dir_p1_u1_L = dir_p1_u1.percan();
  const Dir2 dir_p1_u2_L = dir_p1_u2.percan(); 
  const Dir2 dir_p2_u1 = line_2.point1 - position; 
  const Dir2 dir_p2_u2 = line_2.point2 - position; 

  const Dir2 dir_v = line_2.point1 - line_2.point2;
  lipstick_marks = Dir2 {
    dir_p1_u1.pLd(dir_p2_u1, dir_v),
    dir_p1_u2.pLd(dir_p2_u1, dir_v)
  }.bound01();

  Dir2 middle_kiss = dir_v.madd(lipstick_marks.sum() * -0.5f, dir_p2_u1);

  __m128 bound_p = _mm_set1_ps(0.0001f), bound_n = _mm_set1_ps(-0.0001f);

  __m128 aux = _mm_mul_ps (dir_p1_u2_L.v, dir_p1_u1.v);
  __m128 coef = _mm_rcp_ss(_mm_add_ss (_mm_shuffle_ps (aux, aux, 0b01010101), aux));
  coef = _mm_shuffle_ps (coef, coef, 0);
  __m128 over_both = _mm_and_ps (
    _mm_cmpgt_ps (
      _mm_mul_ps (
        _mm_set_ps (
          0.f, 
          middle_kiss * dir_p1_u2_L, 
          dir_p2_u1 * dir_p1_u2_L, 
          dir_p2_u2 * dir_p1_u2_L
        ), coef
      ), bound_p
    ),
    _mm_cmple_ps (
      _mm_mul_ps (
        _mm_set_ps (
          0.f, 
          middle_kiss * dir_p1_u1_L, 
          dir_p2_u1 * dir_p1_u1_L, 
          dir_p2_u2 * dir_p1_u1_L
        ), coef
      ), bound_n
    )
  );

  return _mm_movemask_ps(over_both) & 0b111;
}

std::vector<MaskObject> generate_view_covering (const Dir2& position, const std::vector<MaskObject>& segments, ViewGeneration by_what) {
  int (*meeting_condition_for_ordering) (
    const SecondLevelElement&, 
    const SecondLevelElement&, 
    const Dir2&
  );
  int (*meeting_condition_for_obfuscating) (
    const SecondLevelElement&, 
    const SecondLevelElement&, 
    const Dir2&, 
    Dir2&
  );
  
  switch(by_what) {
    case ViewGeneration::POINT:
      meeting_condition_for_ordering = meeting_condition_for_ordering_by_point;
      meeting_condition_for_obfuscating = meeting_condition_for_obfuscating_by_point;
      break;
    default:
      meeting_condition_for_ordering = meeting_condition_for_ordering_by_direction;
      meeting_condition_for_obfuscating = meeting_condition_for_obfuscating_by_direction;
      break;
  };

  /* Initialization of the buckets. */
  std::vector<FirstLevelElement> buckets(
    segments.size (),
    FirstLevelElement {
      .data = std::vector<SecondLevelElement>(),
      .first_level_offset = 0,
      .first_second_level_offset = 0,
      .last_second_level_offset = 0
    }
  );
  for (int32_t i = 0; i < (int32_t)segments.size(); i++) {
    buckets[i].data.reserve (BUCKET_LINES_ESTIMATED_PARTITIONS);
    buckets[i].data.push_back (SecondLevelElement {
      .point1 = segments[i].point1,
      .point2 = segments[i].point2,
      .partition_offset = -1
    });
    buckets[i].first_level_offset = i-1;
  }

  uint32_t many_elements = segments.size();

  /* Rejecting volumes. */
  int32_t pos_1 = many_elements - 1;
  int pos_2;
  while (pos_1 >= 0) {

    /* * * * * * * * * * * * * * * * * * * * *
     * searching for the most near segment. *
     * * * * * * * * * * * * * * * * * * * * */

    pos_2 = buckets[pos_1].first_level_offset;
    POS_2_NEXT:
    while (pos_2 >= 0) {

      int32_t inner_pos_1 = buckets[pos_1].first_second_level_offset;
      while (inner_pos_1 >= 0) {
      
        int32_t inner_pos_2 = buckets[pos_2].first_second_level_offset;
        while (inner_pos_2 >= 0) {

          int meet_cond = meeting_condition_for_ordering (
            buckets[pos_1].data[inner_pos_1], 
            buckets[pos_2].data[inner_pos_2], 
            position
          );

          if (meet_cond & 0b1) {
            if (meet_cond & 0b10) {
              pos_2 = buckets[pos_2].first_level_offset;
            } else {
              std::swap (buckets[pos_1].data, buckets[pos_2].data);
              std::swap (buckets[pos_1].last_second_level_offset, buckets[pos_2].last_second_level_offset);
              std::swap (buckets[pos_1].first_second_level_offset, buckets[pos_2].first_second_level_offset);
              pos_2 = buckets[pos_1].first_level_offset;
            }
            goto POS_2_NEXT;
          }
          inner_pos_2 = buckets[pos_2].data[inner_pos_2].partition_offset;
        }
        inner_pos_1 = buckets[pos_1].data[inner_pos_1].partition_offset;
      }
      pos_2 = buckets[pos_2].first_level_offset;
    }

    /* * * * * * * * * * * * * * * * * * 
     * obfuscating submissive segments. *
     * * * * * * * * * * * * * * * * * */

    pos_2 = buckets[pos_1].first_level_offset;
    int32_t prev_pos_2 = pos_1;

    while (pos_2 >= 0) {

      bool more_posibilities = true;
      int32_t inner_pos_1 = buckets[pos_1].first_second_level_offset;
      while (inner_pos_1 >= 0 && more_posibilities) {
      
        int32_t inner_pos_2 = buckets[pos_2].first_second_level_offset;
        int32_t prev_inner_pos_2 = -1;
        while (inner_pos_2 >= 0) {

          SecondLevelElement& line_2 = buckets[pos_2].data[inner_pos_2];
          const Dir2 dir_v = line_2.point2 - line_2.point1;

          Dir2 lipstick_marks;
          int meet_cond = meeting_condition_for_obfuscating (
            buckets[pos_1].data[inner_pos_1], 
            line_2, 
            position, 
            lipstick_marks
          );

          if (lipstick_marks.y > lipstick_marks.x)
            std::swap (lipstick_marks.x, lipstick_marks.y);

          switch (by_what) {
            case ViewGeneration::POINT:
              /* obfuscate one side. */
              if ((meet_cond & 0b11) == 0b10) {
                if (meet_cond & 0b100)
                  line_2.point1 += dir_v * lipstick_marks.x;
                else
                  line_2.point1 += dir_v * lipstick_marks.y;

              } else if ((meet_cond & 0b11) == 0b01) {
                if (meet_cond & 0b100)
                  line_2.point2 = dir_v.madd(lipstick_marks.y, line_2.point1);
                else
                  line_2.point2 = dir_v.madd(lipstick_marks.x, line_2.point1);

                /* obfuscate subsegment(divide the segment in two parts). */
              } else if (meet_cond == 0b100) {
                Dir2 kiss_x = dir_v.madd(lipstick_marks.x, line_2.point1);
                Dir2 kiss_y = dir_v.madd(lipstick_marks.y, line_2.point1);

                if ((line_2.point2 - kiss_x).modulo2() >= 1.f) {
                  int32_t aux = buckets[pos_2].data.size();
                  int32_t last = buckets[pos_2].last_second_level_offset;

                  buckets[pos_2].data.push_back(line_2);

                  buckets[pos_2].last_second_level_offset = 
                    buckets[pos_2].data[last].partition_offset = 
                    aux;
                  buckets[pos_2].data[aux].partition_offset = -1;
                  buckets[pos_2].data[aux].point1 = kiss_x;
                  many_elements++;
                }

                if ((line_2.point1 - kiss_y).modulo2() < 1.f) {
                  if (buckets[pos_2].first_second_level_offset == inner_pos_2)
                    buckets[pos_2].first_second_level_offset = 
                      buckets[pos_2].data[inner_pos_2].partition_offset;
                  else if (buckets[pos_2].data[inner_pos_2].partition_offset == -1) {
                    buckets[pos_2].last_second_level_offset = prev_inner_pos_2;
                    buckets[pos_2].data[prev_inner_pos_2].partition_offset = -1;
                  } else 
                    buckets[pos_2].data[prev_inner_pos_2].partition_offset = 
                      buckets[pos_2].data[inner_pos_2].partition_offset;
                  many_elements--;
                } else {
                  line_2.point2 = kiss_y;
                }

                inner_pos_2 = -1;
                goto FIN_INNER;
              }
              break;
            default:

              /* obfuscate one side. */
              if ((meet_cond & 0b11) == 0b10) {
                line_2.point1 = dir_v.madd(lipstick_marks.x, line_2.point1);

              } else if ((meet_cond & 0b11) == 0b01) {
                line_2.point2 = dir_v.madd(lipstick_marks.y, line_2.point1);

                /* obfuscate subsegment(divide the segment in two parts). */
              } else if (meet_cond == 0b100) {
                Dir2 kiss_x = dir_v.madd(lipstick_marks.x, line_2.point1);
                Dir2 kiss_y = dir_v.madd(lipstick_marks.y, line_2.point1);

                if ((line_2.point2 - kiss_x).modulo2() >= 1.f) {
                  int32_t aux = buckets[pos_2].data.size();
                  int32_t last = buckets[pos_2].last_second_level_offset;

                  buckets[pos_2].data.push_back(line_2);

                  buckets[pos_2].last_second_level_offset = 
                    buckets[pos_2].data[last].partition_offset = 
                    aux;
                  buckets[pos_2].data[aux].partition_offset = -1;
                  buckets[pos_2].data[aux].point1 = kiss_x;
                  many_elements++;
                }

                if ((line_2.point1 - kiss_y).modulo2() < 1.f) {
                  if (buckets[pos_2].first_second_level_offset == inner_pos_2)
                    buckets[pos_2].first_second_level_offset = 
                      buckets[pos_2].data[inner_pos_2].partition_offset;
                  else if (buckets[pos_2].data[inner_pos_2].partition_offset == -1) {
                    buckets[pos_2].last_second_level_offset = prev_inner_pos_2;
                    buckets[pos_2].data[prev_inner_pos_2].partition_offset = -1;
                  } else 
                    buckets[pos_2].data[prev_inner_pos_2].partition_offset = 
                      buckets[pos_2].data[inner_pos_2].partition_offset;
                  many_elements--;
                } else {
                  line_2.point2 = kiss_y;
                }

                inner_pos_2 = -1;
                goto FIN_INNER;
              }
              break;
          }

          /* obfuscate completely. */
          if (meet_cond == 0b111 || (line_2.point1 - line_2.point2).modulo2() < 1.f) {
            if (buckets[pos_2].first_second_level_offset == inner_pos_2)
              inner_pos_2 = 
                buckets[pos_2].first_second_level_offset = 
                buckets[pos_2].data[inner_pos_2].partition_offset;
            else if (buckets[pos_2].data[inner_pos_2].partition_offset == -1) {
              buckets[pos_2].last_second_level_offset = prev_inner_pos_2;
              inner_pos_2 = 
                buckets[pos_2].data[prev_inner_pos_2].partition_offset = 
                -1;
            } else 
              inner_pos_2 =
                buckets[pos_2].data[prev_inner_pos_2].partition_offset = 
                buckets[pos_2].data[inner_pos_2].partition_offset;
            many_elements--;
          } else {
            prev_inner_pos_2 = std::exchange (inner_pos_2, buckets[pos_2].data[inner_pos_2].partition_offset);
          }
        }

        FIN_INNER:
        more_posibilities = buckets[pos_2].first_second_level_offset >= 0;
        inner_pos_1 = buckets[pos_1].data[inner_pos_1].partition_offset;
      }
      if (!more_posibilities)
        pos_2 = 
          buckets[prev_pos_2].first_level_offset = 
            buckets[pos_2].first_level_offset;
      else 
        prev_pos_2 = std::exchange(pos_2, buckets[pos_2].first_level_offset);
    }

    pos_1 = buckets[pos_1].first_level_offset;
  }

  std::vector<MaskObject> ret;
  ret.reserve (many_elements);

  pos_1 = segments.size() - 1;
  while (pos_1 >= 0) {
    int32_t inner_pos_1 = buckets[pos_1].first_second_level_offset;

    while (inner_pos_1 >= 0) {
      SecondLevelElement& line_1 = buckets[pos_1].data[inner_pos_1];
      ret.push_back(MaskObject {
        .point1 = line_1.point1, 
        .point2 = line_1.point2, 
        .circle = false
      });
      inner_pos_1 = buckets[pos_1].data[inner_pos_1].partition_offset;
    } 
    pos_1 = buckets[pos_1].first_level_offset;
  }

  return ret;
}
