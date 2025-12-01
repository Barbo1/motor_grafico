#include "../headers/concepts/lights.hpp"
#include "../headers/primitives/global.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>


void draw_view (Global* glb, Dir2 position, const std::vector<MaskObject>& segments) {

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
        Dir2 dir_u1 = line_1.point1 - position; 
        Dir2 dir_u2 = line_1.point2 - position; 
      
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "linea 1:" << std::endl;
        std::cout << "point1 = (" << line_1.point1.x << ", " << line_1.point1.y << ")" << std::endl;
        std::cout << "point2 = (" << line_1.point2.x << ", " << line_1.point2.y << ")" << std::endl;

        int32_t inner_pos_2 = buckets[pos_2].second_level_offset;
        int32_t prev_inner_pos_2 = -1;
        while (inner_pos_2 > -1 && inner_pos_1 > -1) {

          /* For line 2. */

          MaskLineInfo& line_2 = buckets[pos_2].data[inner_pos_2];
          Dir2 dir_v = line_2.point2 - line_2.point1; 
          Dir2 dir_v_L = dir_v.percan(); 
          Dir2 dir_b_L = (line_2.point1 - position).percan(); 
        
          std::cout << "linea 2:" << std::endl;
        std::cout << "point1 = (" << line_2.point1.x << ", " << line_2.point1.y << ")" << std::endl;
        std::cout << "point2 = (" << line_2.point2.x << ", " << line_2.point2.y << ")" << std::endl;

          float denom_1 = -1.f / (dir_v_L * dir_u1);
          float denom_2 = -1.f / (dir_v_L * dir_u2);
          float d = dir_v * dir_b_L;

          float lipstick_marks_1 = (dir_u1 * dir_b_L) * denom_1;
          float lipstick_marks_2 = (dir_u2 * dir_b_L) * denom_2;
          float d_coef_1 = d * denom_1;
          float d_coef_2 = d * denom_2;

          Dir2 lipstick_marks = Dir2 {lipstick_marks_1, lipstick_marks_2}.bound01();
         
          float lsmMax, lsmMin, d_coefMax, d_coefMin;

          if (lipstick_marks.x > lipstick_marks.y) {
            lsmMax = lipstick_marks.x;
            lsmMin = lipstick_marks.y;
            d_coefMax = d * denom_1;
            d_coefMin = d * denom_2;
          } else {
            lsmMax = lipstick_marks.y;
            lsmMin = lipstick_marks.x;
            d_coefMax = d * denom_2;
            d_coefMin = d * denom_1;
          }

          Dir2 middle_kiss = dir_v * ((lsmMax + lsmMin) * 0.5f) + line_2.point1;

          float q_n_m = ((middle_kiss - position).percan() * dir_u1) / (dir_u2.percan() * dir_u1);
          float r_n_m = ((middle_kiss - position) * dir_u2.percan()) / (dir_u2.percan() * dir_u1);

          float q_n_1 = ((line_2.point1 - position).percan() * dir_u1) / (dir_u2.percan() * dir_u1);
          float r_n_1 = ((line_2.point1 - position) * dir_u2.percan()) / (dir_u2.percan() * dir_u1);

          float q_n_2 = ((line_2.point2 - position).percan() * dir_u1) / (dir_u2.percan() * dir_u1);
          float r_n_2 = ((line_2.point2 - position) * dir_u2.percan()) / (dir_u2.percan() * dir_u1);

          std::cout << std::endl;
          std::cout << "primera parte: " << std::endl;
          std::cout << "q_n_1: " << q_n_1 << std::endl;
          std::cout << "r_n_1: " << r_n_1 << std::endl;
          std::cout << "q_n_2: " << q_n_2 << std::endl;
          std::cout << "r_n_2: " << r_n_2 << std::endl;
          std::cout << "q_n_m: " << q_n_m << std::endl;
          std::cout << "r_n_m: " << r_n_m << std::endl;
          std::cout << "d_coefMax: " << d_coefMax << std::endl;
          std::cout << "d_coefMin: " << d_coefMin << std::endl;
          std::cout << std::endl;
          std::cout << "lsmMax: " << lsmMax << std::endl;
          std::cout << "lsmMin: " << lsmMin << std::endl;
          
          if ((d_coefMin >= 1.f || d_coefMax >= 1.f) && (
             (q_n_1 >= 0.f && r_n_1 >= 0.f) || 
             (q_n_2 >= 0.f && r_n_2 >= 0.f) || 
             (q_n_m >= 0.f && r_n_m >= 0.f)
            )) {

            /* obfuscate completely. */
            if (lsmMax == 1.f && lsmMin == 0.f) {
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

            /* obfuscate one side. */
            } else if (lsmMin == 0.f) {
              if (d_coefMin > 0.f)
                line_2.point1 = dir_v.madd(lsmMax, line_2.point1);
              else 
                line_2.point2 = dir_v.madd(lsmMax, line_2.point1);
              prev_inner_pos_2 = std::exchange (inner_pos_2, buckets[pos_2].data[inner_pos_2].partition_offset);
            
            } else if (lsmMax == 1.f) {
              if (d_coefMax > 0.f)
                line_2.point2 = dir_v.madd(lsmMin, line_2.point1);
              else 
                line_2.point1 = dir_v.madd(lsmMin, line_2.point1);
              prev_inner_pos_2 = std::exchange (inner_pos_2, buckets[pos_2].data[inner_pos_2].partition_offset);
            
            /* divide the line. */
            } else {
              uint32_t aux = buckets[pos_2].data.size();
              uint32_t last = buckets[pos_2].last_second_level_offset;
              buckets[pos_2].last_second_level_offset++;

              buckets[pos_2].data.push_back(line_2);
              buckets[pos_2].data[aux].partition_offset = -1;
              buckets[pos_2].data[last].partition_offset = aux; // esto tiene que se last_offset

              buckets[pos_2].data[aux].point1 += dir_v * lsmMax;
              line_2.point2 = dir_v.madd(lsmMin, line_2.point1);
              inner_pos_2 = -1;
            }
          } else {
            prev_inner_pos_2 = std::exchange (inner_pos_2, buckets[pos_2].data[inner_pos_2].partition_offset);
          }


          /* For line 1. */

          Dir2 dir_v_O = line_1.point2 - line_1.point1; 
          Dir2 dir_v_O_L = dir_v_O.percan(); 
          Dir2 dir_b_O_L = (line_1.point1 - position).percan(); 

          Dir2 dir_u1_O = line_2.point1 - position;
          Dir2 dir_u2_O = line_2.point2 - position;

          float denom_1_O = -1.f / (dir_v_O_L * dir_u1_O);
          float denom_2_O = -1.f / (dir_v_O_L * dir_u2_O);
          float d_O = dir_v_O * dir_b_O_L;

          lipstick_marks_1 = (dir_u1_O * dir_b_O_L) * denom_1_O;
          lipstick_marks_2 = (dir_u2_O * dir_b_O_L) * denom_2_O;
          d_coef_1 = d_O * denom_1_O;
          d_coef_2 = d_O * denom_2_O;

          lipstick_marks = Dir2 {lipstick_marks_1, lipstick_marks_2}.bound01();

          if (lipstick_marks.x > lipstick_marks.y) {
            lsmMax = lipstick_marks.x;
            lsmMin = lipstick_marks.y;
            d_coefMax = d_coef_1;
            d_coefMin = d_coef_2;
          } else {
            lsmMax = lipstick_marks.y;
            lsmMin = lipstick_marks.x;
            d_coefMax = d_coef_2;
            d_coefMin = d_coef_1;
          }

          middle_kiss = dir_v_O * ((lsmMax + lsmMin) * 0.5f) + line_1.point1;

          q_n_m = ((middle_kiss - position).percan() * dir_u1_O) / (dir_u2_O.percan() * dir_u1_O);
          r_n_m = ((middle_kiss - position) * dir_u2_O.percan()) / (dir_u2_O.percan() * dir_u1_O);
                                             
          q_n_1 = ((line_1.point1 - position).percan() * dir_u1_O) / (dir_u2_O.percan() * dir_u1_O);
          r_n_1 = ((line_1.point1 - position) * dir_u2_O.percan()) / (dir_u2_O.percan() * dir_u1_O);
                                             
          q_n_2 = ((line_1.point2 - position).percan() * dir_u1_O) / (dir_u2_O.percan() * dir_u1_O);
          r_n_2 = ((line_1.point2 - position) * dir_u2_O.percan()) / (dir_u2_O.percan() * dir_u1_O);

          std::cout << std::endl;
          std::cout << "segunda parte: " << std::endl;
          std::cout << "q_n_1: " << q_n_1 << std::endl;
          std::cout << "r_n_1: " << r_n_1 << std::endl;
          std::cout << "q_n_2: " << q_n_2 << std::endl;
          std::cout << "r_n_2: " << r_n_2 << std::endl;
          std::cout << "q_n_m: " << q_n_m << std::endl;
          std::cout << "r_n_m: " << r_n_m << std::endl;
          std::cout << "d_coefMax: " << d_coefMax << std::endl;
          std::cout << "d_coefMin: " << d_coefMin << std::endl;
          std::cout << std::endl;
          std::cout << "lsmMax: " << lsmMax << std::endl;
          std::cout << "lsmMin: " << lsmMin << std::endl;
          
          if ((d_coefMin >= 1.f || d_coefMax >= 1.f) && (
             (q_n_1 >= 0.f && r_n_1 >= 0.f) || 
             (q_n_2 >= 0.f && r_n_2 >= 0.f) || 
             (q_n_m >= 0.f && r_n_m >= 0.f)
            )) {

            /* obfuscate completely. */
            if (lsmMax == 1.f && lsmMin == 0.f) {
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

            /* obfuscate one side. */
            } else if (lsmMin == 0.f) {
              if (d_coefMax > 0.f)
                line_1.point1 = dir_v_O.madd(lsmMax, line_1.point1);
              else 
                line_1.point2 = dir_v_O.madd(lsmMax, line_1.point1);
            
            } else if (lsmMax == 1.f) {
              if (d_coefMin > 0.f)
                line_1.point2 = dir_v_O.madd(lsmMin, line_1.point1);
              else
                line_1.point1 = dir_v_O.madd(lsmMin, line_1.point1);
            
            /* divide the line. */
            } else {
              uint32_t aux = buckets[pos_1].data.size();
              uint32_t last = buckets[pos_1].last_second_level_offset;
              buckets[pos_1].last_second_level_offset++;

              buckets[pos_1].data.push_back(line_1);
              buckets[pos_1].data[aux].partition_offset = -1;
              buckets[pos_1].data[last].partition_offset = aux; // esto tiene que se last_offset

              buckets[pos_1].data[aux].point1 += dir_v_O * lsmMax;
              line_1.point2 = dir_v_O.madd(lsmMin, line_1.point1);
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

  pos_1 = initial_offset;
  while (pos_1 > -1) {
    int32_t inner_pos_1 = buckets[pos_1].second_level_offset;

    while (inner_pos_1 > -1) {
      MaskLineInfo& line_1 = buckets[pos_1].data[inner_pos_1];

      SDL_SetRenderDrawColor(glb->get_render(), 255, 0, 0, 255);
      SDL_RenderDrawLine(glb->get_render(), line_1.point1.x, line_1.point1.y, line_1.point2.x, line_1.point2.y);
      SDL_SetRenderDrawColor(glb->get_render(), 0, 0, 255, 255);
      SDL_RenderDrawPoint(glb->get_render(), line_1.point1.x, line_1.point1.y);
      SDL_RenderDrawPoint(glb->get_render(), line_1.point2.x, line_1.point2.y);

      inner_pos_1 = buckets[pos_1].data[inner_pos_1].partition_offset;
    } 
    pos_1 = buckets[pos_1].offset;
  }
}

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, 600, 800, SDL_Color {30, 30, 30, 0});

  Dir2 origen = {617.f, 336.f};
  std::vector<MaskObject> segments = {
    MaskObject {.point1 = Dir2 {134.f, 114.f}, .point2 = Dir2 {52.f, 274.f}, .circle = false},
    MaskObject {.point1 = Dir2 {245.f, 103.f}, .point2 = Dir2 {604.f, 76.f}, .circle = false},
    MaskObject {.point1 = Dir2 {678.f, 149.f}, .point2 = Dir2 {411.f, 178.f}, .circle = false},
    MaskObject {.point1 = Dir2 {117.f, 355.f}, .point2 = Dir2 {278.f, 195.f}, .circle = false},
    MaskObject {.point1 = Dir2 {531.f, 218.f}, .point2 = Dir2 {634.f, 210.f}, .circle = false},
    MaskObject {.point1 = Dir2 {566.f, 441.f}, .point2 = Dir2 {382.f, 313.f}, .circle = false},
    MaskObject {.point1 = Dir2 {617.f, 466.f}, .point2 = Dir2 {388.f, 437.f}, .circle = false},
    MaskObject {.point1 = Dir2 {121.f, 265.f}, .point2 = Dir2 {172.f, 199.f}, .circle = false},
    MaskObject {.point1 = Dir2 {447.f, 337.f}, .point2 = Dir2 {464.f, 348.f}, .circle = false},
    MaskObject {.point1 = Dir2 {498.f, 370.f}, .point2 = Dir2 {506.f, 377.f}, .circle = false},
    MaskObject {.point1 = Dir2 {529.f, 390.f}, .point2 = Dir2 {546.f, 407.f}, .circle = false},
    MaskObject {.point1 = Dir2 {540.f, 320.f}, .point2 = Dir2 {595.f, 355.f}, .circle = false},
    MaskObject {.point1 = Dir2 {}, .point2 = Dir2 {}, .circle = false},
    MaskObject {.point1 = Dir2 {}, .point2 = Dir2 {}, .circle = false},
    MaskObject {.point1 = Dir2 {}, .point2 = Dir2 {}, .circle = false},
  };

  bool cont = true;
  SDL_Event event;

  glb->begin_render();

  /* The delay must be inside. */
  SDL_Delay(1);

  SDL_SetRenderDrawColor(glb->get_render(), 255, 255, 255, 255);
  for (const auto& segment: segments) {
    SDL_RenderDrawLine(glb->get_render(), segment.point1.x, segment.point1.y, segment.point2.x, segment.point2.y);
  }

  draw_view (glb, origen, segments);

  SDL_SetRenderDrawColor(glb->get_render(), 0, 255, 0, 255);
  SDL_RenderDrawPoint(glb->get_render(), origen.x, origen.y);

  glb->end_render();

  while (cont) {
    
    /* Evaluacion de perifericos. */
    if (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          cont = false;
          break;

        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
              cont = false;
              break;
          }
          break;
      }
    }
  } 
}
