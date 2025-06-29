#include "../../headers/concepts/primitives.hpp"
#include <cmath>
#include <utility>

float fisqrt (float x) {
  union {int xi; float xf;};

  xf = x;
  float xhalf = 0.5 * xf;
  xi = 0x5f375a82 - (xi >> 1);
  return xf*(1.5f - xhalf * xf * xf);
}

Direction::Direction () {
  this->x = 0;
  this->y = 0;
}

Direction::Direction (float x, float y) {
  this->x = x;
  this->y = y;
}

Direction::Direction (const Direction & dir) {
  this->x = dir.x;
  this->y = dir.y;
}

Direction::Direction (Direction && dir) {
  this->x = std::exchange(dir.x, 0);
  this->y = std::exchange(dir.y, 0);
}

Direction & Direction::operator= (const Direction & dir) {
  this->x = dir.x;
  this->y = dir.y;
  return *this;
}

Direction & Direction::operator= (Direction && dir) {
  this->x = std::exchange(dir.x, 0);
  this->y = std::exchange(dir.y, 0);
  return *this;
}

bool Direction::operator== (const Direction & dir) {
  return this->x == dir.x && this->y == dir.y;
}

Direction Direction::operator- () {
  return {-this->x, -this->y};
}

template<DirFin R> Direction Direction::operator+ (R d) {
  return Direction (
    this->x + d.x,
    this->y + d.y
  );
}


template<DirFin R> Direction Direction::operator- (R d) {
  return Direction (
    this->x - d.x,
    this->y - d.y
  );
}

Direction Direction::operator* (float f) {
  return Direction (
    this->x * f,
    this->y * f 
  );
}

template<DirFin R> float Direction::operator* (R d) {
  return this->x * d.x + this->y * d.y;
}

template<DirFin R> void Direction::operator+= (R d) {
  this->x += d.x;
  this->y += d.y;
}

template<DirFin R> void Direction::operator-= (R d) {
  this->x -= d.x;
  this->y -= d.y;
}

void Direction::operator*= (float f) {
  this->x *= f;
  this->y *= f;
}

Direction Direction::normalize () {
  float inorm = fisqrt(this->x * this->x + this->y * this->y);
  return Direction ({
    this->x * inorm,
    this->y * inorm
  });
}

float Direction::modulo () {
  return std::sqrt(this->x * this->x + this->y * this->y);
}


/**********************
 *  AngularDirection  *
 *********************/

AngularDirection::AngularDirection () : Direction() {
  this->a = 0;
}
    
AngularDirection::AngularDirection (float x, float y, float a) : Direction (x, y) {
  this->a = a;
}

AngularDirection::AngularDirection (const AngularDirection & adir) : Direction (adir) {
  this->a = adir.a;
}

AngularDirection::AngularDirection (AngularDirection && adir) : Direction (adir) {
  this->a = std::exchange(adir.a, 0);
}

AngularDirection & AngularDirection::operator= (const AngularDirection & adir) {
  this->Direction::operator=(adir);
  this->a = adir.a;
  return *this;
}

AngularDirection & AngularDirection::operator= (AngularDirection && adir) {
  this->Direction::operator=(std::move(adir));
  this->a = adir.a;
  return *this;
}

bool AngularDirection::operator== (const AngularDirection & adir) {
  return this->Direction::operator==(adir) && this->a == adir.a;
}

AngularDirection AngularDirection::operator- () {
  return {-this->x, -this->y, this->a};
}

template<DirFin R> AngularDirection AngularDirection::operator+ (R d) {
  float angle = this->a;
  if constexpr (std::is_same_v<R, AngularDirection>)
    angle += d.a;
  return AngularDirection {
    this->x + d.x,
    this->y + d.y,
    angle
  };
}

template<DirFin R> AngularDirection AngularDirection::operator- (R d) {
  float angle = this->a;
  if constexpr (std::is_same_v<R, AngularDirection>)
    angle -= d.a;
  return AngularDirection {
    this->x - d.x,
    this->y - d.y,
    angle
  };
}

AngularDirection AngularDirection::operator* (float f) {
  return AngularDirection(this->x * f, this->y * f, this->a);
}

template<DirFin R> float AngularDirection::operator* (R d) {
  return this->Direction::operator*(d);
}

template<DirFin R> void AngularDirection::operator+= (R d) {
  this->x += d.x;
  this->y += d.y;
  if constexpr (std::is_same_v<R, AngularDirection>)
    this->a -= d.a;
}

template<DirFin R> void AngularDirection::operator-= (R d) {
  this->x -= d.x;
  this->y -= d.y;
  if constexpr (std::is_same_v<R, AngularDirection>)
    this->a -= d.a;
}


/*******************
 *  Instantiation  *
 *******************/

template Direction Direction::operator+<AngularDirection>(AngularDirection);
template Direction Direction::operator+<Direction>(Direction);
template Direction Direction::operator-<AngularDirection>(AngularDirection);
template Direction Direction::operator-<Direction>(Direction);
template float Direction::operator*<AngularDirection>(AngularDirection);
template float Direction::operator*<Direction>(Direction);
template void Direction::operator+=<AngularDirection>(AngularDirection);
template void Direction::operator+=<Direction>(Direction);
template void Direction::operator-=<AngularDirection>(AngularDirection);
template void Direction::operator-=<Direction>(Direction);

template AngularDirection AngularDirection::operator+<AngularDirection>(AngularDirection);
template AngularDirection AngularDirection::operator+<Direction>(Direction);
template AngularDirection AngularDirection::operator-<AngularDirection>(AngularDirection);
template AngularDirection AngularDirection::operator-<Direction>(Direction);
template float AngularDirection::operator*<AngularDirection>(AngularDirection);
template float AngularDirection::operator*<Direction>(Direction);
template void AngularDirection::operator+=<AngularDirection>(AngularDirection);
template void AngularDirection::operator+=<Direction>(Direction);
template void AngularDirection::operator-=<AngularDirection>(AngularDirection);
template void AngularDirection::operator-=<Direction>(Direction);
