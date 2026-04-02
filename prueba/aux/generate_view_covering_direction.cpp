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
  const Dir2 d_L = d.percan();

  Dir2 lipstick_marks = Dir2 {
    d.pLd (line_1.point1 - line_2.point1, v_2),
    d.pLd (line_1.point2 - line_2.point1, v_2)
  }.bound01();

  Dir2 middle_kiss = v_2.madd (
    _mm_cvtss_f32 (_mm_hadd_ps (lipstick_marks.v, lipstick_marks.v)) * 0.5f, 
    line_2.point1 - line_1.point1
  );

  __m128 dist = _mm_div_ps (
    _mm_set_ps (0.f, 0.f, middle_kiss.percan() * v_1, middle_kiss * d_L), 
    _mm_set1_ps (v_1 * d_L)
  );

  __m128 meet_cond = _mm_cmpgt_ps (dist, _mm_set1_ps (0.0001));
  __m128 over_both = _mm_and_ps (_mm_cmplt_ps (dist, _mm_set1_ps (1.0001)), meet_cond);

  return (_mm_movemask_ps(meet_cond) & 0b0010) | (_mm_movemask_ps(over_both) & 0b0001);
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

  if (lipstick_marks.y > lipstick_marks.x) {
    std::swap (lipstick_marks.x, lipstick_marks.y);
  }

  Dir2 middle_kiss = v_2.madd (
    _mm_cvtss_f32 (_mm_hadd_ps (lipstick_marks.v, lipstick_marks.v)) * 0.5f, 
    line_2.point1
  );
  __m128 bound_under = _mm_set1_ps (0.0001);
  __m128 denom = _mm_rcp_ps (_mm_set1_ps (v_1 * d_L));
  __m128 dist = _mm_mul_ps (_mm_set_ps (
    0.f,
    (middle_kiss - line_1.point1) * d_L,
    (line_2.point1 - line_1.point1) * d_L,
    (line_2.point2 - line_1.point1) * d_L
  ), denom);
  __m128 over_both = _mm_and_ps (
    _mm_cmplt_ps (dist, _mm_set1_ps (1.0001)), 
    _mm_cmpgt_ps (dist, bound_under)
  );

  return (_mm_movemask_ps(over_both) & 0b111);
}

std::vector<MaskObject> generate_view_covering_by_direction (const Dir2& direction, const std::vector<MaskObject>& segments) {

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

          Dir2 lipstick_marks, proportional_distance;
          int meet_cond = meeting_condition_for_ordering_by_direction (
            buckets[pos_1].data[inner_pos_1], 
            buckets[pos_2].data[inner_pos_2], 
            direction
          );

          if (meet_cond & 0b1) {
            if (meet_cond & 0b10) {
              pos_2 = buckets[pos_2].first_level_offset;
              goto POS_2_NEXT;
            } else {
              std::swap (buckets[pos_1].data, buckets[pos_2].data);
              std::swap (buckets[pos_1].last_second_level_offset, buckets[pos_2].last_second_level_offset);
              std::swap (buckets[pos_1].first_second_level_offset, buckets[pos_2].first_second_level_offset);
              pos_2 = buckets[pos_1].first_level_offset;
              goto POS_2_NEXT;
            }
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
          int meet_cond = meeting_condition_for_obfuscating_by_direction (
            buckets[pos_1].data[inner_pos_1], 
            line_2, 
            direction,
            lipstick_marks
          );

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
