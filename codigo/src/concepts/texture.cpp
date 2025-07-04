#include "../../headers/concepts/primitives.hpp"
#include "../../headers/concepts/texture.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <utility>
#include <array>
#include <iostream>

void circle_bound_maker (int radio, int* bounds, int offset) {
  int* biter = bounds;
  const float radio_2 = radio * radio;

  float y = radio-1;
  int x = std::sqrt(radio + y);
  for (; y > 0; y-=1, x = std::sqrt(radio_2 - y*y)) {
    *(biter++) = radio-x;
    *(biter++) = radio+x;
  }
  for (int* rbiter = biter-1; rbiter >= bounds; rbiter-=2) {
    *(biter++) = *(rbiter-1);
    *(biter++) = *rbiter;
  }
  biter = bounds;
  for (int i = 0, sum = offset + 2 * (radio + offset) * offset; i < 2 * radio - 2; i++, sum += 2 * (radio + offset)) {
    *(biter++) += sum;
    *(biter++) += sum;
  }
}

Texture::Texture(SDL_Renderer* render, int height, int width, SDL_Point & center, Uint32* pixels) {
  SDL_Surface* sur = SDL_CreateRGBSurfaceFrom (
    pixels, width, height, 32, width * 4, 
    0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
  );
  this->texture = SDL_CreateTextureFromSurface(render, sur);
  SDL_FreeSurface(sur);
  this->use_count = new int(1);
  this->width = width;
  this->height = height;
  this->center = center;
}

Texture::Texture () {
  this->texture = nullptr;
  this->use_count = new int(0);
}

Texture::Texture (const Texture & texture) {
  this->texture = texture.texture;
  this->width = texture.width;
  this->height = texture.height;
  this->use_count = texture.use_count;
  (*texture.use_count)++;
}

Texture::Texture (Texture && texture) {
  this->texture = std::exchange(texture.texture, nullptr);
  this->height = std::exchange(texture.height, 0);
  this->width = std::exchange(texture.width, 0);
  this->use_count = std::exchange(texture.use_count, new int(0));
}

Texture & Texture::operator= (const Texture & texture) {
  this->width = texture.width;
  this->height = texture.height;
  (*this->use_count)--;
  if ((*this->use_count) == 0) {
    if (this->texture != nullptr)
      SDL_DestroyTexture (this->texture);
    delete this->use_count;
  }
  this->texture = texture.texture;
  this->use_count = texture.use_count;
  return *this;
}

Texture & Texture::operator= (Texture && texture) {
  this->height = std::exchange(texture.height, 0);
  this->width = std::exchange(texture.width, 0);
  (*this->use_count)--;
  if ((*this->use_count) == 0) {
    if (this->texture != nullptr)
      SDL_DestroyTexture (this->texture);
    delete this->use_count;
  }
  this->use_count = std::exchange(texture.use_count, new int(0));
  this->texture = std::exchange(texture.texture, nullptr);
  return *this;
}

Texture::~Texture() {
  (*this->use_count)--;
  if ((*this->use_count) == 0) {
    if (this->texture != nullptr)
      SDL_DestroyTexture (this->texture);
    delete this->use_count;
  }
}

void Texture::set_center (Direction center) {
  this->center.x = center.x;
  this->center.y = center.y;
}

Texture Texture::bounder (SDL_Renderer* render, int* bounds, int height, int width, SDL_Color color, SDL_Point center) {
  Uint32* pixels = new Uint32[height * width];
  int * piter = bounds;

  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  Uint32 col = SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
  Uint32 non = SDL_MapRGBA(format, 0, 0, 0, 0);
  SDL_FreeFormat(format);

  Uint32 current = non;
  bool inside = false;
  for (int i = 0; i < height * width; i++) {
    while (i == *piter) {
      piter++;
      inside = !inside;
      if (inside) current = col;
      else current = non;
    }

    pixels[i] = current;
  }

  Texture ret = Texture(render, height, width, center, pixels);
  delete [] pixels;
  return ret;
}

Texture Texture::square (SDL_Renderer* render, int height, int base, SDL_Color color) {
  Uint32* pixels = new Uint32[height * base];
  
  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  Uint32 col = SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
  SDL_FreeFormat(format);

  for (int pos = 0; pos < height * base; pos++) {
    pixels[pos] = col;
  }
  
  SDL_Point center;
  center.y = height / 2;
  center.x = base/ 2;
    
  Texture ret = Texture(render, height, base, center, pixels);
  delete [] pixels;
  return ret;
}

Texture rounded_square (SDL_Renderer* render, int height, int base, int radio, SDL_Color color) {
  return Texture();
}

Texture Texture::triangle (SDL_Renderer* render, Direction point1, Direction point2, Direction point3, SDL_Color color) {
  if (point2.y < point1.y) {
    std::swap (point2, point1);
  }
  if (point3.y < point2.y) {
    std::swap (point3, point2);
  }
  if (point2.y < point1.y) {
    std::swap (point2, point1);
  }

  Direction max = point1;
  Direction min = point1;
  if (max.x < point2.x) max.x = point2.x;
  else min.x = point2.x;
  if (max.x < point3.x) max.x = point3.x;
  if (min.x > point3.x) min.x = point3.x;

  max.y = point3.y;

  max -= min;
  point1 -= min;
  point2 -= min;
  point3 -= min;

  SDL_Point center;
  Direction pre_center = (point1 + point2 + point3) * (1.f/3.f);
  center.x = pre_center.x;
  center.y = pre_center.y;

  float m1 = (point1.x - point2.x) / (point1.y - point2.y);
  float c1 = point2.x - point2.y * m1;

  float m2 = (point2.x - point3.x) / (point2.y - point3.y);
  float c2 = point3.x - point3.y * m2;

  float m3 = (point1.x - point3.x) / (point1.y - point3.y);
  float c3 = point3.x - point3.y * m3;

  float m4 = m3;
  float c4 = c3;

  if (m3 < m2) {
    std::swap (m1, m3);
    std::swap (c1, c3);
    std::swap (m2, m4);
    std::swap (c2, c4);
  }

  int width = static_cast<int>(max.x);
  int height = static_cast<int>(max.y);
  int total = height * width;

  int* bounds = new int[2 * height + 1];

  float x = 0;
  int n = 0;
  for (n = 0, x = 0; n < static_cast<int>(point2.y); x += 1, n++) {
    bounds[2*n] = static_cast<int>(m3*x + c3) + width * n;
    bounds[2*n + 1] = static_cast<int>(m1*x + c1) + width * n;
  }
  for (x = n = static_cast<int>(point2.y); n < static_cast<int>(point3.y); x += 1, n++) {
    bounds[2*n] = static_cast<int>(m4*x + c4) + width * n;
    bounds[2*n + 1] = static_cast<int>(m2*x + c2) + width * n;
  }

  bounds[2 * height] = total + 1;

  Texture ret = bounder(render, bounds, height, width, color, center);
  delete [] bounds;
  return ret;
}
    
Texture Texture::polygon (SDL_Renderer* render, std::vector<Direction> points, SDL_Color color) {
  /* Searching maximum and minimum coordenates. */
  Direction max(points[0]), min(points[0]);
  for (auto& point: points) {
    if (max.x < point.x) max.x = point.x;
    if (min.x > point.x) min.x = point.x;
    if (max.y < point.y) max.y = point.y;
    if (min.y > point.y) min.y = point.y;
  }

  /* Translate points. */
  for (std::size_t i = 0; i < points.size(); i++)
    points[i] -= min;
  max -= min;

  /* Calculating texture parameter. */
  int height = max.y;
  int width = max.x;
  Direction center;
  for (auto& point: points)
    center += point;
  center *= (1.f/points.size());

  /* Calculating coefitients. */
  auto calc_coef = [] (Direction point0, Direction point1, Direction point2) {
    double mi = (point1.x - point2.x) / (point1.y - point2.y);
    double ci = point2.x - point2.y * mi;
    double q, p;
    if (point2.x != point1.x) {
      q = 1 / (point2.x - point1.x);
      p = (ci - point1.x) * q;
      q = mi * q;
    } else {
      q = 1/ (point2.y - point1.y);
      p = -point1.y * q;
    }
    return std::pair<std::array<double, 4>, bool> {
      {mi, ci - mi, q, p - q}, (point1.y - point0.y) * (point1.y - point2.y) < 0
    };
  };

  std::vector<std::pair<std::array<double, 4>, bool>> coef = std::vector<std::pair<std::array<double, 4>, bool>>();
  coef.reserve (points.size());
  std::size_t i = 1;
  coef.push_back (calc_coef (points[points.size() - 1], points[0], points[1]));
  for (; i < points.size() - 1; i++)
    coef.push_back (calc_coef (points[i-1], points[i], points[i+1]));
  coef.push_back (calc_coef (points[i-1], points[i], points[0]));

  /* Calculate the bound to make the texture. */
  int * bounds = new int[height * points.size()];
  int * biter = bounds;
  int * piter = bounds;
  int * liter;

  for (uint32_t level = 0; level < width * height; level += width) {
    for (auto & mc: coef) {
      double& q = mc.first[3]; q += mc.first[2];
      double& elem = mc.first[1]; elem += mc.first[0];
      if (-0.00001 < q && q < 1.00001) {
        int32_t inter = std::lround(elem) + level;
        if (mc.second) {
          liter = piter;
          while (liter < biter && *liter != inter) { liter++; }
          if (liter < biter) continue; 
        }
        *(biter++) = inter;
      }
    }
    for (; piter < biter; piter++)
      for (liter = piter + 1; liter < biter; liter++)
        if (*liter < *piter)
          std::swap (*piter, *liter); 
  }
  *biter = height * width;
  
  Texture ret = bounder(render, bounds, height, width, color, {static_cast<int>(center.x), static_cast<int>(center.y)});
  delete [] bounds;
  return ret; 
}

Texture Texture::circle (SDL_Renderer* render, int radio, SDL_Color color) {
  int* bounds = new int[4*radio - 4];
  circle_bound_maker (radio, bounds, 0);

  Texture ret = bounder(render, bounds, 2*radio, 2*radio, color, {radio, radio});
  delete [] bounds;
  return ret;
}
   
Texture Texture::circunference (SDL_Renderer* render, int radio, int width, SDL_Color color) {
  std::size_t r1 = radio - width;
  std::size_t b1 = 4*radio - 4;
  std::size_t b2 = 4*r1 - 4;
  int* bounds1 = new int[b1];
  int* bounds2 = new int[b2];
  int* bounds3 = new int[b1 + b2];
  circle_bound_maker (radio, bounds1, 0);
  circle_bound_maker (r1, bounds2, width);

  std::size_t i = 0, j = 0, k = 0;
  while (i < b1 && j < b2) {
    if (bounds1[i] < bounds2[j]) {
      bounds3[k] = bounds1[i++];
    } else if (bounds2[j] < bounds1[i]) {
      bounds3[k] = bounds2[j++];
    } else {
      bounds3[k++] = bounds1[i++];
      bounds3[k] = bounds2[j++];
    }
    k++;
  }
  while (i < b1)
    bounds3[k++] = bounds1[i++];
  delete [] bounds1;
  delete [] bounds2;

  bounds3[k] = 4*radio*radio;

  Texture ret = bounder(render, bounds3, 2*radio, 2*radio, color, {radio, radio});
  delete [] bounds3;
  return ret;
}

Texture Texture::oval (SDL_Renderer* render, int height, int base, SDL_Color color) {
  int* bounds = new int[4*height- 4];
  int* biter = bounds;

  float a_b = static_cast<float>(base) / static_cast<float>(height), b_2 = height * height;
  float y = height-1;
  int x = static_cast<int>(a_b*sqrt(b_2 - y * y)); 
  int level = 0;
  for (; y > 0; y-=1, x = static_cast<int>(a_b*sqrt(b_2 - y * y))) {
    *(biter++) = base-x;
    *(biter++) = base+x;
    level++;
  }
  for (int* rbiter = biter-1; rbiter >= bounds; rbiter-=2) {
    *(biter++) = *(rbiter-1);
    *(biter++) = *rbiter;
    level++;
  }
  biter = bounds;
  for (int i = 0, sum = 0; i < level; i++, sum += 2*base) {
    *(biter++) += sum;
    *(biter++) += sum;
  }

  Texture ret = bounder(render, bounds, 2*height, 2*base, color, {base, height});
  delete [] bounds;
  return ret;
}

void Texture::draw (SDL_Renderer* render, const AngularDirection & position) const {
  SDL_Rect dst;
  dst.x = static_cast<int>(position.x);
  dst.y = static_cast<int>(position.y); 
  dst.h = this->height;
  dst.w = this->width;
  SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
  SDL_RenderCopyEx(render, this->texture, nullptr, &dst, position.a, &this->center, SDL_FLIP_NONE);
}
