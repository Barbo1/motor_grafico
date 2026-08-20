#include "../../../../headers/concepts/lights.hpp"
#include "../../../../headers/primitives/math.hpp"

#include <cmath>
#include <cstdint>
#include <utility>
#include <immintrin.h>
MaskObjectList generate_view_covering (
  DynamicalArena& darena,
  const MaskObjectList& segment_list, 
  const Dir2& position, 
  ViewGeneration by_what
) {

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
    case ViewGeneration::VG_POINT:
      meeting_condition_for_ordering = meeting_condition_for_ordering_by_point;
      meeting_condition_for_obfuscating = meeting_condition_for_obfuscating_by_point;
      break;
    default:
      meeting_condition_for_ordering = meeting_condition_for_ordering_by_direction;
      meeting_condition_for_obfuscating = meeting_condition_for_obfuscating_by_direction;
      break;
  };
  
  auto [segments, segments_size] = segment_list;

  /* Initialization of the buckets. */
  std::vector<FirstLevelElement> buckets(
    segments_size,
    FirstLevelElement {
      .data = std::vector<SecondLevelElement>(),
      .first_level_offset = 0,
      .first_second_level_offset = 0,
      .last_second_level_offset = 0
    }
  );

  int32_t i = 0;
  for (MaskObject* iter = segments; iter != nullptr; iter = iter->next) {
    buckets[i].data.reserve (BUCKET_LINES_ESTIMATED_PARTITIONS);
    buckets[i].data.push_back (SecondLevelElement {
      .point1 = Dir2(iter->point1),
      .point2 = Dir2(iter->point2),
      .partition_offset = -1
    });
    buckets[i].first_level_offset = i-1;
    i++;
  }

  uint32_t many_elements = segments_size;

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

          if (lipstick_marks.y() > lipstick_marks.x())
            lipstick_marks.turn();

          switch (by_what) {
            case ViewGeneration::VG_POINT:
              /* obfuscate one side. */
              if ((meet_cond & 0b11) == 0b10) {
                if (meet_cond & 0b100)
                  line_2.point1 += dir_v * lipstick_marks.x();
                else
                  line_2.point1 += dir_v * lipstick_marks.y();

              } else if ((meet_cond & 0b11) == 0b01) {
                if (meet_cond & 0b100)
                  line_2.point2 = dir_v.madd(lipstick_marks.y(), line_2.point1);
                else
                  line_2.point2 = dir_v.madd(lipstick_marks.x(), line_2.point1);

                /* obfuscate subsegment(divide the segment in two parts). */
              } else if (meet_cond == 0b100) {
                Dir2 kiss_x = dir_v.madd(lipstick_marks.x(), line_2.point1);
                Dir2 kiss_y = dir_v.madd(lipstick_marks.y(), line_2.point1);

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
                line_2.point1 = dir_v.madd(lipstick_marks.x(), line_2.point1);

              } else if ((meet_cond & 0b11) == 0b01) {
                line_2.point2 = dir_v.madd(lipstick_marks.y(), line_2.point1);

                /* obfuscate subsegment(divide the segment in two parts). */
              } else if (meet_cond == 0b100) {
                Dir2 kiss_x = dir_v.madd(lipstick_marks.x(), line_2.point1);
                Dir2 kiss_y = dir_v.madd(lipstick_marks.y(), line_2.point1);

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

  MaskObject* ret = nullptr;
  MaskObject* iter = nullptr;

  pos_1 = segments_size - 1;
  while (pos_1 >= 0) {
    int32_t inner_pos_1 = buckets[pos_1].first_second_level_offset;

    while (inner_pos_1 >= 0) {
      SecondLevelElement& line_1 = buckets[pos_1].data[inner_pos_1];

      MaskObject* aux =  darena.alloc_mo();
      aux->point1.store(line_1.point1);
      aux->point2.store(line_1.point2);

      if (ret == nullptr) {
        iter = ret = aux;
      } else {
        iter->next = aux;
        iter = aux;
      }

      inner_pos_1 = buckets[pos_1].data[inner_pos_1].partition_offset;
    } 
    pos_1 = buckets[pos_1].first_level_offset;
  }

  if (ret != nullptr)
    iter->next = nullptr;

  return MaskObjectList {
    .obj = ret,
    .size = many_elements
  };
}
