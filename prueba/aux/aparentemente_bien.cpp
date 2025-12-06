#include "./../../../headers/concepts/lights.hpp"

#include <cstdint>
#include <utility>
#include <iostream>
#include <bitset>
#include <immintrin.h>

inline int meeting_condition_for_ordering (const MaskLineInfo& line_1, const MaskLineInfo& line_2, const Dir2& position) {
  const Dir2 dir_v = line_2.point2 - line_2.point1;
  const Dir2 dir_p1_u1 = line_1.point1 - position; 
  const Dir2 dir_p1_u2 = line_1.point2 - position; 
  const Dir2 dir_p1_u1_L = dir_p1_u1.percan();
  const Dir2 dir_p1_u2_L = dir_p1_u2.percan(); 
  const Dir2 dir_p2_u1 = line_2.point1 - position; 
  const Dir2 dir_p2_u2 = line_2.point2 - position; 

  Dir2 lipstick_marks = (-Dir2 {
    (dir_p2_u1 * dir_p1_u1_L) / (dir_v * dir_p1_u1_L),
    (dir_p2_u1 * dir_p1_u2_L) / (dir_v * dir_p1_u2_L)
  }).bound01();

  Dir2 middle_kiss = dir_v.madd((lipstick_marks.x + lipstick_marks.y) * 0.5f, line_2.point1);

  __m128 coef = _mm_rcp_ps(_mm_set1_ps (dir_p1_u2_L * dir_p1_u1));
  __m128 over_both = _mm_and_ps (
    _mm_cmpgt_ps (
      _mm_mul_ps (coef, _mm_set_ps (
        0.f,
        (middle_kiss - position) * dir_p1_u2_L, 
        dir_p2_u1 * dir_p1_u2_L,
        dir_p2_u2 * dir_p1_u2_L
      )), _mm_setzero_ps()
    ),
    _mm_cmple_ps (
      _mm_mul_ps (coef, _mm_set_ps (
        0.f,
        (middle_kiss - position) * dir_p1_u1_L, 
        dir_p2_u1 * dir_p1_u1_L,
        dir_p2_u2 * dir_p1_u1_L
      )), _mm_setzero_ps()
    )
  );

  __m128 coef_1 = _mm_rcp_ps(_mm_set1_ps ((line_1.point2 - line_1.point1).percan() * dir_p1_u1));
  __m128 coef_2 = _mm_rcp_ps(_mm_set1_ps ((line_1.point1 - line_1.point2).percan() * dir_p1_u2));
  __m128 meet_cond = _mm_and_ps (
    _mm_and_ps(
      _mm_cmpgt_ps (
        _mm_mul_ps (coef_2, _mm_set_ps (
          0.f,
          (middle_kiss - line_1.point2) * (line_1.point1 - line_1.point2).percan(),
          (line_2.point1 - line_1.point2) * (line_1.point1 - line_1.point2).percan(),
          (line_2.point2 - line_1.point2) * (line_1.point1 - line_1.point2).percan()
        )), _mm_setzero_ps()
      ),
      _mm_cmple_ps (
        _mm_mul_ps (coef_2, _mm_set_ps (
          0.f,
          (middle_kiss - line_1.point2) * dir_p1_u2_L,
          (line_2.point1 - line_1.point2) * dir_p1_u2_L,
          (line_2.point2 - line_1.point2) * dir_p1_u2_L
        )), _mm_setzero_ps()
      )
    ),
    _mm_and_ps(
      _mm_cmpgt_ps (
        _mm_mul_ps (coef_1, _mm_set_ps (
          0.f,
          (middle_kiss - line_1.point1) * (line_1.point2 - line_1.point1).percan() ,
          (line_2.point1 - line_1.point1) * (line_1.point2 - line_1.point1).percan(),
          (line_2.point2 - line_1.point1) * (line_1.point2 - line_1.point1).percan()
        )), _mm_setzero_ps()
      ),
      _mm_cmple_ps (
        _mm_mul_ps (coef_1, _mm_set_ps (
          0.f,
          (middle_kiss - line_1.point1) * dir_p1_u1_L,
          (line_2.point1 - line_1.point1) * dir_p1_u1_L,
          (line_2.point2 - line_1.point1) * dir_p1_u1_L
        )), _mm_setzero_ps()
      )
    )
  );

  return (_mm_movemask_ps(meet_cond) & 0b111) << 3 | _mm_movemask_ps(over_both);
}

inline int meeting_condition_for_obfuscating (
  const MaskLineInfo& line_1, const MaskLineInfo& line_2, const Dir2& position, 
  Dir2& lipstick_marks
) {

  const Dir2 dir_v = line_2.point2 - line_2.point1;
  const Dir2 dir_p1_u1 = line_1.point1 - position; 
  const Dir2 dir_p1_u2 = line_1.point2 - position; 
  const Dir2 dir_p1_u1_L = dir_p1_u1.percan();
  const Dir2 dir_p1_u2_L = dir_p1_u2.percan(); 
  const Dir2 dir_p2_u1 = line_2.point1 - position; 
  const Dir2 dir_p2_u2 = line_2.point2 - position; 

  lipstick_marks = (-Dir2 {
    (dir_p2_u1 * dir_p1_u1_L) / (dir_v * dir_p1_u1_L),
    (dir_p2_u1 * dir_p1_u2_L) / (dir_v * dir_p1_u2_L)
  }).bound01();

  if (lipstick_marks.y > lipstick_marks.x) {
    std::swap (lipstick_marks.y, lipstick_marks.x);
  }

  Dir2 middle_kiss = dir_v.madd((lipstick_marks.x + lipstick_marks.y) * 0.5f, line_2.point1);

  __m128 coef = _mm_rcp_ps(_mm_set1_ps (dir_p1_u2_L * dir_p1_u1));
  __m128 over_both = _mm_and_ps (
    _mm_cmpgt_ps (
      _mm_mul_ps (coef, _mm_set_ps (
        0.f,
        (middle_kiss - position) * dir_p1_u2_L, 
        dir_p2_u1 * dir_p1_u2_L,
        dir_p2_u2 * dir_p1_u2_L
      )), _mm_setzero_ps()
    ),
    _mm_cmple_ps (
      _mm_mul_ps (coef, _mm_set_ps (
        0.f,
        (middle_kiss - position) * dir_p1_u1_L, 
        dir_p2_u1 * dir_p1_u1_L,
        dir_p2_u2 * dir_p1_u1_L
      )), _mm_setzero_ps()
    )
  );

  return _mm_movemask_ps(over_both);
}

std::vector<MaskObject> generate_view_covering (Dir2 position, const std::vector<MaskObject>& segments) {

  /* Initialization of the buckets. */
  std::vector<FirstLevelElement> buckets(
    segments.size (),
    FirstLevelElement {
      .data = std::vector<MaskLineInfo>(),
      .first_level_offset = 0,
      .first_second_level_offset = 0,
      .last_second_level_offset = 0
    }
  );
  for (int32_t i = 0; i < (int32_t)segments.size(); i++) {
    buckets[i].data.reserve (BUCKET_LINES_ESTIMATED_PARTITIONS);
    buckets[i].data.push_back (MaskLineInfo {
      .point1 = segments[i].point1,
      .point2 = segments[i].point2,
      .partition_offset = -1
    });
    buckets[i].first_level_offset = i-1;
  }

  uint32_t many_elements = segments.size();

  std::cout << ">>>>>>>>>>>>>>>>>>>>>___________________>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;

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
          int meet_cond = meeting_condition_for_ordering (
            buckets[pos_1].data[inner_pos_1], 
            buckets[pos_2].data[inner_pos_2], 
            position
          );
          std::cout << std::endl << std::endl << "pos_1: " << pos_1 << ", pos_2: " << pos_2 << std::endl;
          std::cout << std::endl << std::endl << "inner_pos_1: " << inner_pos_1 << ", inner_pos_2: " << inner_pos_2 << std::endl;
          std::cout << "line_1: (" 
            << buckets[pos_1].data[inner_pos_1].point1.x << ", " 
            << buckets[pos_1].data[inner_pos_1].point1.y << ") -> (" 
            << buckets[pos_1].data[inner_pos_1].point2.x << ", " 
            << buckets[pos_1].data[inner_pos_1].point2.y << ")" 
            << std::endl;
          std::cout << "line_2: (" 
            << buckets[pos_2].data[inner_pos_2].point1.x << ", " 
            << buckets[pos_2].data[inner_pos_2].point1.y << ") -> (" 
            << buckets[pos_2].data[inner_pos_2].point2.x << ", " 
            << buckets[pos_2].data[inner_pos_2].point2.y << ")" 
            << std::endl;
          std::cout << "position: (" 
            << position.x << ", " 
            << position.y << ")" 
            << std::endl;

          if (meet_cond & 0b111) {
            if (meet_cond & 0b111000) {
              pos_2 = buckets[pos_2].first_level_offset;
              std::cout << "dominante: line_1" << std::endl;
              goto POS_2_NEXT;
            } else {
              std::swap (buckets[pos_1].data, buckets[pos_2].data);
              std::swap (buckets[pos_1].last_second_level_offset, buckets[pos_2].last_second_level_offset);
              std::swap (buckets[pos_1].first_second_level_offset, buckets[pos_2].first_second_level_offset);
              std::cout << "dominante: line_2" << std::endl;
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

    std::cout << "REALIZO" << std::endl;

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

          MaskLineInfo& line_2 = buckets[pos_2].data[inner_pos_2];

          Dir2 lipstick_marks;
          int meet_cond = meeting_condition_for_obfuscating (
            buckets[pos_1].data[inner_pos_1], 
            line_2, 
            position, 
            lipstick_marks
          );
          
          std::cout << std::endl << std::endl << pos_1 << ", " << pos_2 << std::endl;
          std::cout << "line_1: (" 
            << buckets[pos_1].data[inner_pos_1].point1.x << ", " 
            << buckets[pos_1].data[inner_pos_1].point1.y << ") -> (" 
            << buckets[pos_1].data[inner_pos_1].point2.x << ", " 
            << buckets[pos_1].data[inner_pos_1].point2.y << ")" 
            << std::endl;
          std::cout << "line_2: (" 
            << buckets[pos_2].data[inner_pos_2].point1.x << ", " 
            << buckets[pos_2].data[inner_pos_2].point1.y << ") -> (" 
            << buckets[pos_2].data[inner_pos_2].point2.x << ", " 
            << buckets[pos_2].data[inner_pos_2].point2.y << ")" 
            << std::endl;
          std::cout << "position: (" 
            << position.x << ", " 
            << position.y << ")" 
            << std::endl;
          std::cout << "La condicion retorno el siguiente numero: " << std::bitset<4>(meet_cond) << std::endl;

          const Dir2 dir_v = line_2.point2 - line_2.point1;

          /* obfuscate one side. */
          if ((meet_cond & 0b11) == 0b10) {
            if (lipstick_marks.x < 1.f)
              line_2.point1 += dir_v * lipstick_marks.x;
            else if (lipstick_marks.y > 0.f)
              line_2.point1 += dir_v * lipstick_marks.y;

          } else if ((meet_cond & 0b11) == 0b01) {
            if (lipstick_marks.x < 1.f)
              line_2.point2 = dir_v.madd(lipstick_marks.x, line_2.point1);
            else if (lipstick_marks.y > 0.f)
              line_2.point2 = dir_v.madd(lipstick_marks.y, line_2.point1);
          
          /* obfuscate subsegment(divide the segment in two parts). */
          } else if (meet_cond == 0b100) {
            int32_t aux = buckets[pos_2].data.size();
            int32_t last = buckets[pos_2].last_second_level_offset;

            Dir2 new_point_2 = dir_v.madd(lipstick_marks.y, line_2.point1);
            Dir2 new_point_1 = dir_v.madd(lipstick_marks.x, line_2.point1);

            if ((line_2.point2 - new_point_1).modulo2() < 1.f) {
              buckets[pos_2].data.push_back(line_2);
              buckets[pos_2].last_second_level_offset = 
                buckets[pos_2].data[last].partition_offset = 
                  aux;
              buckets[pos_2].data[aux].partition_offset = -1;
              buckets[pos_2].data[aux].point1 = new_point_1;
              many_elements++;
            }

            if ((line_2.point1 - new_point_2).modulo2() < 1.f) {
              if (buckets[pos_2].first_second_level_offset == inner_pos_2)
                inner_pos_2 = 
                  buckets[pos_2].first_second_level_offset = 
                    buckets[pos_2].data[inner_pos_2].partition_offset;
              else if (buckets[pos_2].data[inner_pos_2].partition_offset == -1) {
                buckets[pos_2].last_second_level_offset = prev_inner_pos_2;
                buckets[pos_2].data[prev_inner_pos_2].partition_offset = -1;
                inner_pos_2 = -1;
              } else 
                inner_pos_2 =
                  buckets[pos_2].data[prev_inner_pos_2].partition_offset = 
                    buckets[pos_2].data[inner_pos_2].partition_offset;
              many_elements--;
            } else {
              line_2.point2 = new_point_2;
            }
            
            inner_pos_2 = -1;
            inner_pos_1 = -1;

            goto FIN_ITER;
          }
  
          /* obfuscate completely. */
          if (meet_cond == 0b111 || (line_2.point1 - line_2.point2).modulo2() < 1.f) {
            if (buckets[pos_2].first_second_level_offset == inner_pos_2)
              inner_pos_2 = 
                buckets[pos_2].first_second_level_offset = 
                  buckets[pos_2].data[inner_pos_2].partition_offset;
            else if (buckets[pos_2].data[inner_pos_2].partition_offset == -1) {
              buckets[pos_2].last_second_level_offset = prev_inner_pos_2;
              buckets[pos_2].data[prev_inner_pos_2].partition_offset = -1;
              inner_pos_2 = -1;
            } else 
              inner_pos_2 =
                buckets[pos_2].data[prev_inner_pos_2].partition_offset = 
                  buckets[pos_2].data[inner_pos_2].partition_offset;
            many_elements--;
          } else if (inner_pos_2 >= 0) {
            prev_inner_pos_2 = std::exchange (inner_pos_2, buckets[pos_2].data[inner_pos_2].partition_offset);
          }
          FIN_ITER:
        }

        /* arreglar problema:
         *  - cuando se modifica el segmento(en los dos ultimos casos), 
         *      es posible que los puntos sean iguales o que los segmentos
         *      sean demasiado pequeños como para considerarlos.
         * */

        more_posibilities = buckets[pos_2].first_second_level_offset >= 0;
        inner_pos_1 = (inner_pos_1 >= 0 ? buckets[pos_1].data[inner_pos_1].partition_offset : -1);
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
  while (pos_1 > -1) {
    int32_t inner_pos_1 = buckets[pos_1].first_second_level_offset;

    while (inner_pos_1 > -1) {
      MaskLineInfo& line_1 = buckets[pos_1].data[inner_pos_1];
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
