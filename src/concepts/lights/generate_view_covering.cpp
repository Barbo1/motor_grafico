#include "./../../../headers/concepts/lights.hpp"
#include <algorithm>
#include <cstdint>
#include <immintrin.h>

static void max_min (Dir2& h, Dir2& j) {
  __m128 mine = _mm_loadl_pi (_mm_undefined_ps(), (__m64*)&h);
  __m128 other = _mm_loadl_pi (_mm_undefined_ps(), (__m64*)&j);
  __m128 opr = _mm_cmpgt_ps (mine, other);
  opr = _mm_shuffle_ps(opr, opr, _MM_SHUFFLE(0, 0, 0, 0));
  _mm_storel_pi((__m64*)&h, _mm_xor_ps(_mm_and_ps (opr, mine), _mm_andnot_ps (opr, other)));
  _mm_storel_pi((__m64*)&j, _mm_xor_ps(_mm_andnot_ps (opr, mine), _mm_and_ps (opr, other)));
}

std::vector<MaskObject> generate_view_covering (Dir2 position, const std::vector<MaskObject>& segments) {

  /* Copy lines to an internal buffer. */
  std::vector<MaskLineInfo> lines (segments.size(), MaskLineInfo {});
  for (uint32_t i = 0; i < segments.size(); i++) {
    MaskLineInfo& line = lines[i];
    Dir2 point1 = segments[i].point1;
    Dir2 point2 = segments[i].point2;

    line.point1 = point1;
    line.point2 = point2;

    line.center_distance = (((point1 + point2) * 0.5f) - position).modulo();
    line.partition_offset = -1;
  }

  /* Sorting lines by distance to the point. */
  std::sort(
    lines.begin(), 
    lines.end(), 
    [](const MaskLineInfo& elem1, const MaskLineInfo& elem2) {return elem1.center_distance > elem2.center_distance;}
  );

  /* Initialization of the buckets. */
  std::vector<FirstLevelElement> buckets(
    segments.size(), 
    FirstLevelElement{
      .data = std::vector<MaskLineInfo>(),
      .offset = 0,
      .second_level_offset = 0,
      .last_second_level_offset = 0
    }
  );
  for (int32_t i = 0; i < (int32_t)segments.size(); i++) {
    FirstLevelElement& bucket = buckets[i];
    bucket.data.reserve(BUCKET_LINES_ESTIMATED_PARTITIONS);
    bucket.data.push_back(lines[i]);
    bucket.offset = i-1;
  }

  int32_t initial_offset = (int32_t)segments.size() - 1;
  int32_t many_elements = segments.size();

  /* Rejecting volumes. */
  int32_t pos_1 = initial_offset;
  while (pos_1 > -1) {
    int32_t pos_2 = buckets[pos_1].offset;
    int32_t prev_pos_2 = pos_1;

    while (pos_2 > -1) {
      int32_t inner_pos_1 = buckets[pos_1].second_level_offset;
      int32_t prev_inner_pos_1 = -1;

      while (inner_pos_1 > -1 && buckets[pos_1].second_level_offset >= 0 && buckets[pos_2].second_level_offset >= 0) {
        MaskLineInfo& line_1 = buckets[pos_1].data[inner_pos_1];

        int32_t inner_pos_2 = buckets[pos_2].second_level_offset;
        int32_t prev_inner_pos_2 = -1;
        while (inner_pos_2 > -1 && inner_pos_1 > -1) {
          MaskLineInfo& line_2 = buckets[pos_2].data[inner_pos_2];

          const Dir2 dir_p1_u1 = line_1.point1 - position; 
          const Dir2 dir_p1_u2 = line_1.point2 - position; 
          const Dir2 dir_p1_u1_L = dir_p1_u1.percan();
          const Dir2 dir_p1_u2_L = dir_p1_u2.percan(); 

          /* For line 2. */
          Dir2 dir_v = line_2.point2 - line_2.point1;
          Dir2 dir_b_L = (line_2.point1 - position).percan();

          float d = dir_v * dir_b_L;

          Dir2 lipstick_mark_max = (Dir2 {dir_p1_u1 * dir_b_L, d} / (dir_v * dir_p1_u1_L)).bound01();
          Dir2 lipstick_mark_min = (Dir2 {dir_p1_u2 * dir_b_L, d} / (dir_v * dir_p1_u2_L)).bound01();
          max_min (lipstick_mark_max, lipstick_mark_min);

          Dir2 middle_kiss = dir_v.madd(
            (lipstick_mark_max.x + lipstick_mark_min.x) * 0.5f, 
            line_2.point1 - position
          );

          __m128 coef = _mm_set1_ps (dir_p1_u2_L * dir_p1_u1);
          __m128 res = _mm_andnot_ps (
            _mm_xor_ps (coef, _mm_set_ps (
              middle_kiss * dir_p1_u2_L, 
              (line_2.point1 - position) * dir_p1_u2_L,
              (line_2.point2 - position) * dir_p1_u2_L,
              0.f
            )), 
            _mm_xor_ps (coef, _mm_set_ps (
              middle_kiss * dir_p1_u1_L, 
              (line_2.point1 - position) * dir_p1_u1_L,
              (line_2.point2 - position) * dir_p1_u1_L,
              0.f
            ))
          );
          
          if ((lipstick_mark_max.y == 1.f || lipstick_mark_min.y == 1.f) && _mm_movemask_ps (res) > 1) {

            /* obfuscate completely. */
            if (lipstick_mark_max.x == 1.f && lipstick_mark_min.x == 0.f) {
              if (prev_inner_pos_2 == -1)
                inner_pos_2 = 
                  buckets[pos_2].second_level_offset = 
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
            } else if (lipstick_mark_min.x == 0.f) {
              if (lipstick_mark_min.y > 0.f)
                line_2.point1 += dir_v * lipstick_mark_max.x;
              else 
                line_2.point2 = dir_v.madd(lipstick_mark_max.x, line_2.point1);
              prev_inner_pos_2 = std::exchange (inner_pos_2, buckets[pos_2].data[inner_pos_2].partition_offset);
            
            } else if (lipstick_mark_max.x == 1.f) {
              if (lipstick_mark_max.y > 0.f)
                line_2.point2 = dir_v.madd(lipstick_mark_min.x, line_2.point1);
              else
                line_2.point1 += dir_v * lipstick_mark_min.x;
              prev_inner_pos_2 = std::exchange (inner_pos_2, buckets[pos_2].data[inner_pos_2].partition_offset);
            
            /* divide the line. */
            } else {
              uint32_t aux = buckets[pos_2].data.size();
              uint32_t last = buckets[pos_2].last_second_level_offset;
              buckets[pos_2].last_second_level_offset++;

              buckets[pos_2].data.push_back(line_2);
              buckets[pos_2].data[aux].partition_offset = -1;
              buckets[pos_2].data[last].partition_offset = aux; // esto tiene que se last_offset

              buckets[pos_2].data[aux].point1 += dir_v * lipstick_mark_max.x;
              line_2.point2 = dir_v.madd(lipstick_mark_min.x, line_2.point1);
              inner_pos_2 = -1;
              many_elements++;
            }
          } else {
            prev_inner_pos_2 = std::exchange (inner_pos_2, buckets[pos_2].data[inner_pos_2].partition_offset);
          }


          /* For line 1. */

          Dir2 dir_v_O = line_1.point2 - line_1.point1; 
          Dir2 dir_b_O = line_1.point1 - position; 
          Dir2 dir_b_O_L = dir_b_O.percan(); 

          Dir2 dir_u1_O = line_2.point1 - position;
          Dir2 dir_u2_O = line_2.point2 - position;
          Dir2 dir_u1_O_L = dir_u1_O.percan();

          float d_O = dir_v_O * dir_b_O_L;

          lipstick_mark_max = (Dir2 {dir_u1_O * dir_b_O_L, d_O} / (dir_v_O * dir_u1_O_L)).bound01();
          lipstick_mark_min = (Dir2 {dir_u2_O * dir_b_O_L, d_O} / (dir_v_O * dir_u1_O_L)).bound01();
          max_min (lipstick_mark_max, lipstick_mark_min);

          middle_kiss = dir_v_O.madd (
            (lipstick_mark_max.x + lipstick_mark_min.x) * 0.5f, 
            dir_b_O
          );

          coef = _mm_set1_ps (dir_u2_O.percan() * dir_u1_O);
          res = _mm_andnot_ps (
            _mm_xor_ps (coef, _mm_set_ps (
              middle_kiss * dir_u2_O.percan(), 
              dir_b_O * dir_u2_O.percan(),
              (line_1.point2 - position) * dir_u2_O.percan(),
              0.f
            )),
            _mm_xor_ps (coef, _mm_set_ps (
              middle_kiss * dir_u1_O_L, 
              dir_b_O * dir_u1_O_L,
              (line_1.point2 - position) * dir_u1_O_L,
              0.f
            ))
          );
          
          if ((lipstick_mark_max.y == 1.f || lipstick_mark_min.y == 1.f) && _mm_movemask_ps (res) > 1) {
          
            /* obfuscate completely. */
            if (lipstick_mark_max.x == 1.f && lipstick_mark_min.x == 0.f) {
              if (prev_inner_pos_1 == -1)
                inner_pos_1 = 
                  buckets[pos_1].second_level_offset = 
                    buckets[pos_1].data[inner_pos_1].partition_offset;
              else if (buckets[pos_1].last_second_level_offset == pos_1) {
                buckets[pos_1].last_second_level_offset = prev_inner_pos_1;
                buckets[pos_1].data[prev_inner_pos_1].partition_offset = -1;
                inner_pos_1 = -1;
              } else 
                inner_pos_1 =
                  buckets[pos_1].data[prev_inner_pos_1].partition_offset = 
                    buckets[pos_1].data[inner_pos_1].partition_offset;
              many_elements--;

            /* obfuscate one side. */
            } else if (lipstick_mark_min.x == 0.f) {
              if (lipstick_mark_min.y > 0.f)
                line_1.point1 += dir_v_O * lipstick_mark_max.x;
              else 
                line_1.point2 = dir_v_O.madd(lipstick_mark_max.x, line_1.point1);
            
            } else if (lipstick_mark_max.x == 1.f) {
              if (lipstick_mark_max.y > 0.f)
                line_1.point2 = dir_v_O.madd(lipstick_mark_min.x, line_1.point1);
              else
                line_1.point1 += dir_v_O * lipstick_mark_min.x;
            
            /* divide the line. */
            } else {
              uint32_t aux = buckets[pos_1].data.size();
              uint32_t last = buckets[pos_1].last_second_level_offset;
              buckets[pos_1].last_second_level_offset++;

              buckets[pos_1].data.push_back(line_1);
              buckets[pos_1].data[aux].partition_offset = -1;
              buckets[pos_1].data[last].partition_offset = aux; // esto tiene que se last_offset

              buckets[pos_1].data[aux].point1 += dir_v_O * lipstick_mark_max.x;
              line_1.point2 = dir_v_O.madd(lipstick_mark_min.x, line_1.point1);
              many_elements++;
            }
          }
        }

        inner_pos_1 = 
          inner_pos_1 >= 0 ? buckets[pos_1].data[inner_pos_1].partition_offset : -1;
      }

      if (buckets[pos_2].second_level_offset < 0)
        pos_2 = buckets[prev_pos_2].offset = buckets[pos_2].offset;
      else 
        prev_pos_2 = std::exchange(pos_2, buckets[pos_2].offset);

      if (buckets[pos_1].second_level_offset < 0) {
        if (pos_1 == initial_offset)
          initial_offset = pos_1 = buckets[pos_1].offset;
        else
          pos_1 = buckets[pos_1].offset;
        goto POS_1_FIN;
      }
    }

    POS_1_FIN:
    pos_1 = buckets[pos_1].offset;
  }

  std::vector<MaskObject> ret;
  ret.reserve (many_elements);

  pos_1 = initial_offset;
  while (pos_1 > -1) {
    int32_t inner_pos_1 = buckets[pos_1].second_level_offset;

    while (inner_pos_1 > -1) {
      MaskLineInfo& line_1 = buckets[pos_1].data[inner_pos_1];
      ret.push_back(MaskObject {
        .point1 = line_1.point1, 
        .point2 = line_1.point2, 
        .circle = false
      });
      inner_pos_1 = buckets[pos_1].data[inner_pos_1].partition_offset;
    } 
    pos_1 = buckets[pos_1].offset;
  }

  return ret;
}
