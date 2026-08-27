#include "../headers/concepts/global.hpp"
#include "../headers/primitives/arena.hpp"
#include "../headers/concepts/visualizer.hpp"
#include "../headers/concepts/image_modifier.hpp"
#include "../headers/concepts/collision.hpp"
#include "../headers/concepts/lights.hpp"
#include "../headers/pr_objects/square.hpp"
#include "../headers/pr_objects/line.hpp"
#include "../headers/pr_objects/projectile.hpp"
#include "../headers/sp_objects/particle_source.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <iostream>
#include <cmath>

void configure_glyph_system (GlyphsSystem* gs) {
  SDL_Color color = SDL_Color {255,255,255,255};
  constexpr std::array<char16_t, 26 + 26 + 10 + 8> characters = {
    u'A', u'B', u'C', u'D', u'E', u'F', u'G', u'H', u'I', u'J',
    u'K', u'L', u'M', u'N', u'O', u'P', u'Q', u'R', u'S', u'T',
    u'U', u'V', u'W', u'X', u'Y', u'Z',

    u'a', u'b', u'c', u'd', u'e', u'f', u'g', u'h', u'i', u'j',
    u'k', u'l', u'm', u'n', u'o', u'p', u'q', u'r', u's', u't',
    u'u', u'v', u'w', u'x', u'y', u'z',

    u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7', u'8', u'9',

    u'_', u'?', u'!', u':', u',', u'.', u'/', u' '
  };
  for (const auto& character: characters)
    gs->cache(character, 20, color);
  gs->clear_meta();
}

Dir2 calculate_bound_curr_direction (
  const Dir2& bullet_max_begin, 
  const float& min_cos_angle_direction, 
  int x_mouse, 
  int y_mouse
) {
  Dir2 direction = (Dir2(x_mouse, y_mouse) - bullet_max_begin).normalize();
  float cos = std::max(std::abs(direction.x()), min_cos_angle_direction);
  return Dir2(cos, std::copysign(std::sqrt(1 - std::max(std::min(cos*cos, 1.f), 0.f)), direction.y()));
}

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, SDL_Color {30, 30, 30, 0});
  Arena arena = Arena(1000*1000*8);
  DynamicalArena darena = DynamicalArena(10000*8);

  uint32_t width = glb->get_width();
  uint32_t height = glb->get_height();
  Dir2 screen_dims = Dir2 (static_cast<int>(width), static_cast<int>(height));
  
  int32_t error;
  GlyphsSystem gs (glb, &arena, "../fuentes_letras/Nostard-Medium.ttf", &error);
  if (error < 0) {
    std::cout << "problema al cargar fuentes de letra." << std::endl;
    return -1;
  }
  configure_glyph_system (&gs);

  const Uint8* key_array = SDL_GetKeyboardState(nullptr);
  bool cont = true;
  SDL_Event event;

  /* Creacion de objetos movibles. */
  SDL_Color color = SDL_Color {255,255,255,255};
  SDL_Color red = SDL_Color {255,0,0,255};
  SDL_Color cian = SDL_Color {0,255,255,255};

  /* limites del escenario */
  std::array<Line, 4> lines = std::array<Line, 4> {
    Line (Dir2 (0.f, 0.f), Dir2 ((float)width, 0.f)),
    Line (Dir2 (0.f, 0.f), Dir2 (0.f, (float)height)),
    Line (Dir2 ((float)width, (float)height), Dir2 ((float)width, 0.f)),
    Line (Dir2 ((float)width, (float)height), Dir2 (0.f, (float)height))
  };

  struct SquareElement {
    Visualizer<D2FIG> texture;
    Square physical_body;
  };
  struct Canion {
    Visualizer<D2FIG> rotation_engine;
    float width, height;
  };
 

  // informacion del jugados
  
  SquareElement player = SquareElement {
    .texture = ImageModifier::square(40, 40, color).cast(glb),
    .physical_body = Square(AngDir2 (20.f, 20.f, 0.f), 20, 20, 2.f)
  };
  Canion player_canion = Canion {
    .rotation_engine = ImageModifier::circle(arena, 10, color).cast(glb),
    .width = 4.f,
    .height = 20.f
  };



  // informacion del cajas

  std::array<SquareElement, 2> boxes = std::array<SquareElement, 2> {
    SquareElement {
      .texture = ImageModifier::square(200, 100, color).cast(glb),
      .physical_body = Square(AngDir2 (600.f, 400.f, 0.f), 100, 50, 200.f, false)
    },
    SquareElement {
      .texture = ImageModifier::square(80, 80, color).cast(glb),
      .physical_body = Square(AngDir2 (300.f, 200.f, 0.f), 40, 40, 200.f, false)
    }
  };


  // bullets.
  
  struct Bullet {
    ParticleSource<PS_EXPLOSION, 4, FT_LINEAR_N, FT_LINEAR_N> particles;
    Visualizer<D2FIG> texture;
    Projectile physical_body;
    uint32_t many_collisions;
    bool present;
  };
  std::array<Bullet, 200> bullets;

  const float bullet_radio = 4.f;
  const float bullet_mass = 50.f;
  Visualizer<D2FIG> bullet_img = ImageModifier::circle(arena, bullet_radio, cian).cast(glb);
  Visualizer<D2FIG> bullet_particles_img = ImageModifier::square(2, 2, cian).cast(glb);

  for (uint32_t i = 0; i < bullets.size(); i++) {
    bullets[i] = Bullet {
      .particles = ParticleSource<PS_EXPLOSION, 4, FT_LINEAR_N, FT_LINEAR_N> (
        glb,
        AngDir2 (0.f, 0.f, 0.f),
        std::pair<float, float>{M_PI * 0.5f, M_PI * 1.5f},
        bullet_particles_img,
        5,
        0.5f,
        500
      ),
      .texture = bullet_img,
      .physical_body = Projectile (Dir2(), bullet_radio, bullet_mass),
      .many_collisions = 1,
      .present = false
    };
  }


  // Enemies.

  struct FlyingOrbs {
    Visualizer<D2FIG> texture;
    Circle physical_body;
    int32_t life;
    int32_t ticks_until_shot;
    bool present;
  };
  const uint32_t ticks_until_shot_flying_orbs = 5000;
  std::array<FlyingOrbs, 10> enemies;
  
  {
    uint32_t gen_rand = glb->get_random() & 7;
    float radio = static_cast<float>(gen_rand + 30);
    enemies[0] = FlyingOrbs {
      .texture = ImageModifier::circle(arena, radio, red).cast(glb),
      .physical_body = Circle (Dir2(600.f, 100.f), radio, 2.f),
      .life = 1 + (gen_rand > 4),
      .ticks_until_shot = ticks_until_shot_flying_orbs,
      .present = true 
    };
    enemies[0].physical_body.velocity.store(Dir2(0.01f, 0.1f));

    gen_rand = glb->get_random() & 7;
    radio = static_cast<float>(gen_rand + 30);
    enemies[1] = FlyingOrbs {
      .texture = ImageModifier::circle(arena, radio, red).cast(glb),
      .physical_body = Circle (Dir2(400.f, 400.f), radio, 2.f),
      .life = 1 + (gen_rand > 4),
      .ticks_until_shot = ticks_until_shot_flying_orbs,
      .present = true 
    };
    enemies[1].physical_body.velocity.store(Dir2(0.01f, -0.1f));
  }


  // luces.
  MaskObjectList segments = {.obj = nullptr, .size = 0};
  for (const auto& box: boxes) {
    Dir2 pos = Dir2(box.physical_body.position);
    Dir2 D = Dir2(box.physical_body.dims);
    Dir2 P = Dir2(box.physical_body.dims).dir_mul(Dir2(-1.f, 1.f));

    MaskObject* ret_new = darena.alloc_mo();
    ret_new->point1.store(pos + D);
    ret_new->point2.store(pos + P);
    ret_new->next = segments.obj;
    segments.obj = ret_new;

    ret_new = darena.alloc_mo();
    ret_new->point1.store(pos + D);
    ret_new->point2.store(pos - P);
    ret_new->next = segments.obj;
    segments.obj = ret_new;
    
    ret_new = darena.alloc_mo();
    ret_new->point1.store(pos - D);
    ret_new->point2.store(pos + P);
    ret_new->next = segments.obj;
    segments.obj = ret_new;

    ret_new = darena.alloc_mo();
    ret_new->point1.store(pos - D);
    ret_new->point2.store(pos - P);
    ret_new->next = segments.obj;
    segments.obj = ret_new;

    segments.size += 4;
  }

  std::vector<std::pair<Light, MaskObjectList>> light_info = {
    std::pair<Light, MaskObjectList>{{
      .focal_line = std::array<Dir2, 2>{Dir2(150.f, 10.f), Dir2(200.f, 10.f)},
      .position = Dir2(175.f, -15.f),
      .color = {.r = 1.0f, .g = 1.0f, .b = 1.0f},
      .intensity = 100.f,
      .attenuation = 0.005f,
      .type = LightType::LT_FOCALIZED
    }, segments
  }, {{
      .focal_line = std::array<Dir2, 2>(),
      .position = Dir2(175.f, 10.f),
      .color = {.r = 1.0f, .g = 1.0f, .b = 1.0f},
      .intensity = 60.f,
      .attenuation = 0.1f,
      .type = LightType::LT_CENTERD
  }, segments
  }, {{
      .focal_line = std::array<Dir2, 2>(),
      .position = Dir2(0.f, 0.f),
      .color = {.r = 1.0f, .g = .0f, .b = .0f},
      .intensity = 70.f,
      .attenuation = 0.01f,
      .type = LightType::LT_CENTERD
  }, segments
  }, {{
      .focal_line = std::array<Dir2, 2>(),
      .position = Dir2(0.f, 0.f),
      .color = {.r = 1.0f, .g = .0f, .b = .0f},
      .intensity = 70.f,
      .attenuation = 0.01f,
      .type = LightType::LT_CENTERD
    }, segments
  }};
  
  ViewMask view_0 (glb->get_width(), glb->get_height());


  // general information in the program.

  float player_vel = 2.2f;
  float amplitud_pos_margin = 0.365f; // max = 0.365f
  float shot_aperture_angle = M_PI / 18.f;
  float bullet_velocity_norm = 1.7f;
  uint32_t max_bullet_quantity = 100;
  int32_t ratio_bullet_generation_ms = 300;

  // program.
  int x_mouse, y_mouse;
  float real_player_vel = player_vel / (2.f * amplitud_pos_margin);
  float middle_pos_margin = 0.535f;
  float player_pos_margin = 0.5f;
  uint32_t many_bullets = max_bullet_quantity;
  int32_t time_to_next_bullet = ratio_bullet_generation_ms;
  float min_cos_angle_direction = std::cos(shot_aperture_angle);
  float aux_time_1 = 1.f, avg_time_1 = 0.f;
  while (cont) {
    glb->time_bound();

    /* Evaluacion de eventos. */
    while (SDL_PollEvent(&event));
    if (key_array[SDL_SCANCODE_ESCAPE])
      cont = false;
    if (key_array[SDL_SCANCODE_W])
      player_pos_margin = std::max(player_pos_margin - real_player_vel*glb->get_time(), 0.f);
    if (key_array[SDL_SCANCODE_S])
      player_pos_margin = std::min(player_pos_margin + real_player_vel*glb->get_time(), 1.f);

    // calculate view direction.
    uint32_t mask = SDL_GetMouseState(&x_mouse, &y_mouse);
    Dir2 bullet_max_begin = Dir2(2.f*player_canion.width + 2.f, 0.f) + Dir2(player.physical_body.position);
    Dir2 direction = calculate_bound_curr_direction (
      bullet_max_begin, 
      min_cos_angle_direction, 
      x_mouse, 
      y_mouse
    );

    /* Render of the objects. */
    glb->begin_render();
      // set initial position.
      player.physical_body.position.store(Dir2(
        width * 0.04, 
        height * (std::fmaf(std::fmaf(player_pos_margin, 2.f, -1.f), amplitud_pos_margin, middle_pos_margin))
      ));

      Dir2 relative = Dir2(player.physical_body.position) + Dir2(25.f, -1.f);
      Dir2 vech = direction * player_canion.height;
      Dir2 vecw = direction.percan() * player_canion.width;

      Dir2 canion_up_1 = vech + vecw + relative;
      Dir2 canion_up_2 = vech - vecw + relative;
      Dir2 canion_down_1 = -vecw + relative;
      Dir2 canion_down_2 = vecw + relative;
      Dir2 canion_mouth = (canion_up_1 + canion_up_2) * 0.5f;
      std::vector<Dir2> player_canion_aux = std::vector<Dir2>{
        canion_up_1, 
        canion_up_2, 
        canion_down_1, 
        canion_down_2
      };
      print_polygon_c (glb, arena, player_canion_aux, color);
      player_canion.rotation_engine.draw(glb, relative);
      player.texture.draw(glb, player.physical_body.position);
      
      for (auto& box: boxes)
        box.texture.draw(glb, box.physical_body.position);

      for (auto& enemy: enemies)
        if (enemy.present)
          enemy.texture.draw(glb, enemy.physical_body.position);

      for (auto& bullet: bullets) {
        if (bullet.present)
          bullet.texture.draw(glb, bullet.physical_body.position);
        else if (bullet.particles.bursting())
          bullet.particles.draw();
      }

      light_info[0].first.focal_line[0] = canion_up_1;
      light_info[0].first.focal_line[1] = canion_up_2;
      light_info[0].first.position = direction.madd(player_canion.height - 10.f, relative);
      light_info[1].first.position = canion_mouth;

      if (enemies[0].present)
        light_info[2].first.position = enemies[0].physical_body.position;
      if (enemies[1].present)
        light_info[3].first.position = enemies[1].physical_body.position;
      
      view_0.draw_light_mask (arena, darena, light_info, screen_dims, 200);
      glb->apply_mask (view_0);

      std::string aux_str = std::to_string(many_bullets) + " : " + std::to_string(max_bullet_quantity);
      gs.print (aux_str, 20, color, Dir2(20.f, 20.f));
      
      float a = glb->get_time();
      aux_time_1 += 1;
      avg_time_1 += a;
      std::string aux_str_3 = "tiempo: " + std::to_string(a) + ", avg: " + std::to_string(avg_time_1 / aux_time_1);
      gs.print (aux_str_3, 20, color, Dir2(width - 300.f, 80.f));
    glb->end_render();


    // posicionar balas.
    
    bool clicking = mask & SDL_BUTTON(SDL_BUTTON_LEFT);
    if (clicking && time_to_next_bullet == 0.f && many_bullets > 0) {
      bool finded = false;
      for (uint32_t i = 0; i < bullets.size() && !finded; i++) {
        if (!bullets[i].present && !bullets[i].particles.bursting()) {
          finded = true;
          many_bullets--;

          Dir2 bullet_begin = canion_mouth + direction * bullet_radio;
          bullets[i].physical_body = Projectile (bullet_begin, bullet_radio, bullet_mass);
          bullets[i].many_collisions = 1;
          bullets[i].present = true;
          bullets[i].physical_body.velocity.store(direction * bullet_velocity_norm);
        }
      }
      time_to_next_bullet = ratio_bullet_generation_ms;
    }


    // moving the movible obejcts.

    for (auto& enemy: enemies) {
      if (enemy.present) {
        enemy.physical_body.calculate_movement(glb, AngDir2());
        int32_t ticks = enemy.ticks_until_shot - glb->get_ticks();
        enemy.ticks_until_shot = std::max(0, ticks);
      }
    }

    /* Testing of the collitions. */
    for (uint32_t j = 0; j < enemies.size(); j++) {
      auto& enemy = enemies[j];
      if (enemy.present) {
        for (uint32_t i = j + 1; i < enemies.size(); i++)
          if (enemies[i].present && test_collision(enemies[i].physical_body, enemy.physical_body))
            resolve_collision(enemies[i].physical_body, enemy.physical_body);
        for (uint32_t i = 0; i < lines.size(); i++)
          if (test_collision(lines[i], enemy.physical_body))
            resolve_collision(lines[i], enemy.physical_body);
        for (uint32_t i = 0; i < boxes.size(); i++)
          if (test_collision(enemy.physical_body, boxes[i].physical_body))
            resolve_collision(enemy.physical_body, boxes[i].physical_body, true);
      }
    }
    for (auto& bullet: bullets) {
      if (bullet.present) {
        bullet.physical_body.calculate_future_movement(glb, AngDir2());

        bool collide = false;
        for (uint32_t i = 0; i < lines.size(); i++)
         collide |= test_collision(bullet.physical_body, lines[i]);
        for (uint32_t i = 0; i < boxes.size(); i++)
          collide |= test_collision(bullet.physical_body, boxes[i].physical_body);
        for (uint32_t i = 0; i < enemies.size(); i++)
          if (enemies[i].present)
            collide |= test_collision(bullet.physical_body, enemies[i].physical_body);

        bullet.physical_body.make_movement();

        if (collide) {
          bullet.present = false;

          Dir2 pos = bullet.physical_body.position;
          bullet.particles.set_position(pos);
          bullet.particles.burst();

          uint32_t i = 0;
          bool collide_with_enemies = true;
          while (collide_with_enemies && i < enemies.size()) {
            void* ptr1 = bullet.physical_body.col_obj;
            void* ptr2 = static_cast<void*>(&(enemies[i].physical_body));
            if (enemies[i].present && ptr1 == ptr2) {
              enemies[i].life--;
              enemies[i].present = enemies[i].life != 0;
              collide_with_enemies = false;
            }
            i++;
          }
        }
      } else if (bullet.particles.bursting())
        bullet.particles.calculate_movement(AngDir2());
    }
    
    time_to_next_bullet = std::max(time_to_next_bullet - static_cast<int32_t>(glb->get_ticks()), 0);
  }
}
