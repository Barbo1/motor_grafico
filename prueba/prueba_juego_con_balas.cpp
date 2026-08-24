#include "../headers/primitives/global.hpp"
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
#include <ranges>

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

    u'_', u'?', u'!', u':', u',', u'.', u'/'
  };
  for (const auto& character: characters)
    gs->cache(character, 20, color);
  gs->clear_meta();
}

int main () {
  std::string name = "Ventana";
  Global* glb = Global::create(name, SDL_Color {30, 30, 30, 0});
  Arena arena = Arena(1000*1000*8);
  uint32_t width = glb->get_width();
  uint32_t height = glb->get_height();
  
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
        0.01f,
        3000
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
    enemies[0].physical_body.velocity.store(Dir2(0.01f, 0.01f));

    gen_rand = glb->get_random() & 7;
    radio = static_cast<float>(gen_rand + 30);
    enemies[1] = FlyingOrbs {
      .texture = ImageModifier::circle(arena, radio, red).cast(glb),
      .physical_body = Circle (Dir2(400.f, 400.f), radio, 2.f),
      .life = 1 + (gen_rand > 4),
      .ticks_until_shot = ticks_until_shot_flying_orbs,
      .present = true 
    };
    enemies[1].physical_body.velocity.store(Dir2(0.01f, -0.01f));
  }


  // general information in the program.

  float player_vel = 0.2f;
  float amplitud_pos_margin = 0.365f; // max = 0.365f
  float shot_aperture_angle = M_PI / 18.f;
  float bullet_velocity_norm = 0.02f;
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
  while (cont) {
    std::string aux_str = std::to_string(many_bullets) + " : " + std::to_string(max_bullet_quantity);
    time_to_next_bullet = std::max(0, time_to_next_bullet - static_cast<int32_t>(glb->get_ticks()));

    /* Evaluacion de eventos. */
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
      }
    }

    if (key_array[SDL_SCANCODE_W])
      player_pos_margin = std::max(player_pos_margin - real_player_vel*glb->get_time(), 0.f);
    if (key_array[SDL_SCANCODE_S])
      player_pos_margin = std::min(player_pos_margin + real_player_vel*glb->get_time(), 1.f);

    // calculate view direction.
    uint32_t mask = SDL_GetMouseState(&x_mouse, &y_mouse);
    Dir2 direction;
    {
      Dir2 dev = Dir2(2.f*player_canion.width + 2.f, 0.f);
      Dir2 bullet_max_begin = Dir2(player.physical_body.position) + dev;
      direction = (Dir2(x_mouse, y_mouse) - bullet_max_begin).normalize();
      float cos = std::max(std::abs(direction.x()), min_cos_angle_direction);
      direction = Dir2(cos, std::copysign(std::sqrt(1 - std::max(std::min(cos*cos, 1.f), 0.f)), direction.y()));
    }

    // posicionar balas.
    bool clicking = mask & SDL_BUTTON(SDL_BUTTON_LEFT);
    if (clicking && time_to_next_bullet == 0 && many_bullets > 0) {
      bool finded = false;
      for (uint32_t i = 0; i < bullets.size() && !finded; i++) {
        if (!bullets[i].present && !bullets[i].particles.bursting()) {
          finded = true;
          many_bullets--;

          Dir2 dev = Dir2(bullet_radio + player.physical_body.dims.x + 2.f, 0.f);
          Dir2 bullet_begin = Dir2(player.physical_body.position) + dev;
          bullets[i].physical_body = Projectile (bullet_begin, bullet_radio, bullet_mass);
          bullets[i].many_collisions = 1;
          bullets[i].present = true;
          bullets[i].physical_body.velocity.store(direction * bullet_velocity_norm);
        }
      }
      time_to_next_bullet = ratio_bullet_generation_ms;
    }

    /* Render of the objects. */
    glb->begin_render();
      gs.print (aux_str, 20, color, Dir2(20.f, 20.f));
      
      // set initial position.
      player.physical_body.position.store(Dir2(
        width * 0.04, 
        height * (std::fmaf(std::fmaf(player_pos_margin, 2.f, -1.f), amplitud_pos_margin, middle_pos_margin))
      ));

      Dir2 relative = Dir2(player.physical_body.position) + Dir2(20.f, 0.f);
      Dir2 vech = direction * player_canion.height;
      Dir2 vecw = direction.percan() * player_canion.width;
      std::vector<Dir2> player_canion_aux = std::vector<Dir2>{
        vech + vecw + relative + Dir2(5.f, -1.f), 
        vech - vecw + relative + Dir2(5.f, -1.f), 
        -vecw + relative + Dir2(5.f, -1.f), 
        vecw + relative + Dir2(5.f, -1.f)
      };
      print_polygon_c (glb, arena, player_canion_aux, red);
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

    glb->end_render();

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
  }
}
