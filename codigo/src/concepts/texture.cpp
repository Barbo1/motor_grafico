#include "../../headers/concepts/primitives.hpp"
#include "../../headers/concepts/texture.hpp"

#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <utility>
#include <cstring>

Texture::Texture(SDL_Renderer* render, int height, int width, SDL_Point & center, Uint32* pixels) {
  SDL_Surface* sur = SDL_CreateRGBSurfaceFrom(
    pixels, width, height, 32, width * 4, 
    0x000000FF,0x0000FF00,0x00FF0000,0xFF000000
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

  for (int y = 0; y < height; y++) {
    for (int pos = y*base; pos < (y+1)*base; pos++) {
      pixels[pos] = col;
    }
  }
  
  SDL_Point center;
  center.y = height / 2;
  center.x = base/ 2;
    
  Texture ret = Texture(render, height, base, center, pixels);
  delete [] pixels;
  return ret;
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

  max = max - min;
  point1 = point1 - min;
  point2 = point2 - min;
  point3 = point3 - min;

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

Texture Texture::circle (SDL_Renderer* render, int radio, SDL_Color color) {
  int* bounds = new int[4*radio - 4];
  int* biter = bounds;
  const float radio_2 = radio * radio;

  float y = radio-1;
  int x = std::sqrt(radio + y);
  int level = 0;
  for (; y > 0; y-=1, x = std::sqrt(radio_2 - y*y)) {
    *(biter++) = radio-x;
    *(biter++) = radio+x;
    level++;
  }
  for (int* rbiter = biter-1; rbiter >= bounds; rbiter-=2) {
    *(biter++) = *(rbiter-1);
    *(biter++) = *rbiter;
    level++;
  }
  biter = bounds;
  for (int i = 0, sum = 0; i < level; i++, sum += 2*radio) {
    *(biter++) += sum;
    *(biter++) += sum;
  }

  Texture ret = bounder(render, bounds, 2*radio, 2*radio, color, {radio, radio});
  delete [] bounds;
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
