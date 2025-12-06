#include "./../../../headers/concepts/lights.hpp"
#include <cstdint>
#include <immintrin.h>
#include <utility>
#include <iostream>

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

  uint32_t many_elements = segments.size() - 1;

  /* Rejecting volumes. */
  int32_t pos_1 = many_elements;
  while (pos_1 > -1) {
    int32_t pos_2 = buckets[pos_1].first_level_offset;
    int32_t prev_pos_2 = pos_1;

    while (pos_2 > -1) {
      POS_1_CHANGED:

      bool more_posibilities = true;
      int32_t inner_pos_1 = buckets[pos_1].first_second_level_offset;
      while (inner_pos_1 > -1 && more_posibilities) {
      
        int32_t inner_pos_2 = buckets[pos_2].first_second_level_offset;
        int32_t prev_inner_pos_2 = -1;
        while (inner_pos_2 > -1) {
          MaskLineInfo& line_1 = buckets[pos_1].data[inner_pos_1];
          MaskLineInfo& line_2 = buckets[pos_2].data[inner_pos_2];

          const Dir2 dir_p1_u1 = line_1.point1 - position; 
          const Dir2 dir_p1_u2 = line_1.point2 - position; 
          const Dir2 dir_p1_u1_L = dir_p1_u1.percan();
          const Dir2 dir_p1_u2_L = dir_p1_u2.percan(); 
          
          /*****************/
          /* Begin Kissing */
          /*****************/

          const Dir2 dir_v = line_2.point2 - line_2.point1;
          const Dir2 dir_b = line_2.point1 - position;
          const Dir2 dir_b_L = dir_b.percan();

          Dir2 lipstick_marks = (-Dir2 {
            (dir_b * dir_p1_u1_L) / (dir_v * dir_p1_u1_L),
            (dir_b * dir_p1_u2_L) / (dir_v * dir_p1_u2_L)
          }).bound01();

          Dir2 proportional_distance = Dir2 {
            (dir_v * dir_b_L) / (dir_v * dir_p1_u1_L),
            (dir_v * dir_b_L) / (dir_v * dir_p1_u2_L)
          };

          if (lipstick_marks.y > lipstick_marks.x) {
            std::swap(lipstick_marks.x, lipstick_marks.y);
            std::swap(proportional_distance.x, proportional_distance.y);
          }

          Dir2 middle_kiss_v = dir_v.madd((lipstick_marks.x + lipstick_marks.y) * 0.5f,  dir_b);


          __m128 coef = _mm_set1_ps ((line_1.point2 - line_1.point1).percan() * (line_1.point1 - position));

          __m128 meet_cond_1 = _mm_andnot_ps (
            _mm_xor_ps (coef, _mm_set_ps (
              (line_1.point2 - line_1.point1).percan() * middle_kiss_v,
              (line_1.point2 - line_1.point1).percan() * (line_2.point1 - line_1.point1),
              (line_1.point2 - line_1.point1).percan() * (line_2.point2 - line_1.point1),
              0.f
            )), 
            _mm_xor_ps (coef, _mm_set_ps (
              ((line_1.point1 - position) * middle_kiss_v.percan()),
              ((line_1.point1 - position) * (line_2.point1 - line_1.point1).percan()),
              ((line_1.point1 - position) * (line_2.point2 - line_1.point1).percan()),
              0.f
            ))
          );

          coef = _mm_set1_ps ((line_1.point1 - line_1.point2).percan() * (line_1.point2 - position));
          __m128 meet_cond_2 = _mm_andnot_ps (
            _mm_xor_ps (coef, _mm_set_ps (
              ((line_1.point1 - line_1.point2).percan() * middle_kiss_v),
              ((line_1.point1 - line_1.point2).percan() * (line_2.point1 - line_1.point2)),
              ((line_1.point1 - line_1.point2).percan() * (line_2.point2 - line_1.point2)),
              0.f
            )), 
            _mm_xor_ps (coef, _mm_set_ps (
              ((line_1.point2 - position) * middle_kiss_v.percan()),
              ((line_1.point2 - position) * (line_2.point1 - line_1.point2).percan()),
              ((line_1.point2 - position) * (line_2.point2 - line_1.point2).percan()),
              0.f
            ))
          );

          if ((_mm_movemask_ps (meet_cond_1) & _mm_movemask_ps (meet_cond_2)) > 1) { 

            /* line_1 is dominant. */
            if (proportional_distance.y >= 1.f || proportional_distance.x >= 1.f) {

              /* obfuscate completely. */
              if (lipstick_marks.x == 1.f && lipstick_marks.y == 0.f) {
                if (prev_inner_pos_2 == -1)
                  inner_pos_2 = 
                    buckets[pos_2].first_second_level_offset = 
                      buckets[pos_2].data[inner_pos_2].partition_offset;
                else if (buckets[pos_2].last_second_level_offset == pos_2) {
                  buckets[pos_2].last_second_level_offset = prev_inner_pos_2;
                  buckets[pos_2].data[prev_inner_pos_2].partition_offset = -1;
                  inner_pos_2 = -1;
                } else 
                  inner_pos_2 =
                    buckets[pos_2].data[prev_inner_pos_2].partition_offset = 
                      buckets[pos_2].data[inner_pos_2].partition_offset;
                many_elements--;

              /* obfuscate one side. */
              } else if (lipstick_marks.x == 1.f && proportional_distance.x > 0.f) {
                line_2.point2 = dir_v.madd(lipstick_marks.y, line_2.point1);
                prev_inner_pos_2 = std::exchange (inner_pos_2, buckets[pos_2].data[inner_pos_2].partition_offset);

              } else if (proportional_distance.y < 0.f) {
                line_2.point2 = dir_v.madd(lipstick_marks.x, line_2.point1);
                prev_inner_pos_2 = std::exchange (inner_pos_2, buckets[pos_2].data[inner_pos_2].partition_offset);

              } else if (lipstick_marks.y == 0.f && proportional_distance.y > 0.f) {
                line_2.point1 += dir_v * lipstick_marks.x;
                prev_inner_pos_2 = std::exchange (inner_pos_2, buckets[pos_2].data[inner_pos_2].partition_offset);

              } else if (proportional_distance.x < 0.f) {
                line_2.point1 += dir_v * lipstick_marks.y;
                prev_inner_pos_2 = std::exchange (inner_pos_2, buckets[pos_2].data[inner_pos_2].partition_offset);
              
              /* divide the line. */
              } else {
                int32_t aux = buckets[pos_2].data.size();
                int32_t last = buckets[pos_2].last_second_level_offset;
                buckets[pos_2].data.push_back(line_2);

                buckets[pos_2].last_second_level_offset = 
                  buckets[pos_2].data[last].partition_offset = 
                    aux;
                buckets[pos_2].data[aux].partition_offset = -1;

                buckets[pos_2].data[aux].point1 += dir_v * lipstick_marks.x;
                line_2.point2 = dir_v.madd(lipstick_marks.y, line_2.point1);
                inner_pos_2 = -1;
                many_elements++;
              }

            /* line_2 is dominant(other cases doesn't apply). */
            } else {
              std::swap (buckets[pos_1].data, buckets[pos_2].data);
              std::swap (buckets[pos_1].last_second_level_offset, buckets[pos_2].last_second_level_offset);
              std::swap (buckets[pos_1].first_second_level_offset, buckets[pos_2].first_second_level_offset);
              goto POS_1_CHANGED;
            }
          
          } else {
            prev_inner_pos_2 = std::exchange (inner_pos_2, buckets[pos_2].data[inner_pos_2].partition_offset);
          }
          
          /***************/
          /* End Kissing */
          /***************/
        }

        more_posibilities = buckets[pos_2].first_second_level_offset >= 0;
        inner_pos_1 = buckets[pos_1].data[inner_pos_1].partition_offset;
      }
      if (!more_posibilities)
        pos_2 = buckets[prev_pos_2].first_level_offset = buckets[pos_2].first_level_offset;
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
