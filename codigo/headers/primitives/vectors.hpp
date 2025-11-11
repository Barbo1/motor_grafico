#pragma once 

#include "./operations.hpp"
#include <type_traits>
#include <utility>
#include <cmath>

/* This classes are used to represent vectors in the two and three dimentional space. 
 * */

class Dir2;
class AngDir2;
class Dir3;

template<class T>
concept DirFin = std::is_same_v<std::remove_cvref_t<T>, AngDir2> || std::is_same_v<std::remove_cvref_t<T>, Dir2>;

template<class T>
concept DirFin3 = std::is_same_v<std::remove_cvref_t<T>, Dir3>;

class Dir2 {
  public:
    float x;
    float y;

    /* Basic operations. */
    inline Dir2 () noexcept: x(0.f), y(0.f) {}
    inline Dir2 (float x, float y) noexcept: x(x), y(y) {}
    inline Dir2 (const Dir2 & dir) noexcept: x(dir.x), y(dir.y) {}
    inline Dir2 (Dir2 && dir) noexcept: x(dir.x), y(dir.y) {}

    inline Dir2 & operator= (const Dir2 & dir) noexcept {
      this->x = dir.x;
      this->y = dir.y;
      return *this;
    }

    inline Dir2 & operator= (Dir2 && dir) noexcept {
      this->x = dir.x;
      this->y = dir.y;
      return *this;
    }

    /* Casting. */
    inline Dir2 (const Dir3& dir3) noexcept;
    inline Dir2 (const AngDir2& dir) noexcept;

    /* Comparations. */
    template<DirFin R>
    inline bool operator== (R&& dir) const {
      return this->x == dir.x && this->y == dir.y;
    }

    template<DirFin R>
    inline bool operator!= (R&& dir) const {
      return this->x != dir.x || this->y != dir.y;
    }

    /* Operators by overloading. */
    template<typename Self>
    inline auto operator- (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x = -self.x;
        self.y = -self.y;
        return std::forward<Self>(self);
      } else return Dir2 {-self.x, -self.y};
    }

    template<typename Self, DirFin R>
    inline auto operator- (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x -= dir.x;
        self.y -= dir.y;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.x -= self.x;
        dir.y -= self.y;
        return std::forward<R>(dir);
      } else return Dir2 {
        self.x - dir.x,
        self.y - dir.y
      };
    }

    template<DirFin R>
    inline Dir2& operator-= (R&& dir) {
      this->x -= dir.x;
      this->y -= dir.y;
      return *this;
    }
    
    template<typename Self, DirFin R>
    inline auto operator+ (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x += dir.x;
        self.y += dir.y;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.x += self.x;
        dir.y += self.y;
        return std::forward<R>(dir);
      } else return Dir2 {
        self.x + dir.x,
        self.y + dir.y
      };
    }

    template<DirFin R> 
    inline Dir2& operator+= (R&& d) {
      this->x += d.x;
      this->y += d.y;
      return *this;
    }
    
    template<DirFin R>
    inline float operator* (R&& dir) const {
      return this->x * dir.x + this->y * dir.y; 
    }

    template<typename Self>
    inline auto operator* (this Self&& self, const float& number) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x *= number;
        self.y *= number;
        return std::forward<Self>(self);
      } else return Dir2 {
        self.x * number,
        self.y * number
      };
    }

    inline Dir2& operator*= (const float& number) {
      this->x *= number;
      this->y *= number;
      return *this;
    }

    /* Operations by functions. */
    template<typename Self>
    inline auto percan (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>){
        std::swap(self.x, self.y);
        self.y = -self.y;
        return std::forward<Self>(self);
      } else return Dir2 {-self.y, self.x};
    }

    inline void rotate (const float& angle) {
      float sina = std::sin (angle);
      float cosa = std::cos (angle);
      float newx = this->x * cosa - this->y * sina;
      this->y = this->x * sina + this->y * cosa;
      this->x = newx;
    }

    template<typename Self>
    inline auto normalize (this Self&& self) {
      float inorm = fisqrt(self.x * self.x + self.y * self.y);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x *= inorm;
        self.y *= inorm;
        return std::forward<Self>(self);
      } else return Dir2 {
        self.x * inorm,
        self.y * inorm
      };
    }

    template<typename Self>
    inline auto abs (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        absv(&self.x);
        absv(&self.y);
        return std::forward<Self>(self);
      } else return Dir2 {
        absv (self.x),
        absv (self.y)
      };
    }

    template<DirFin R>
    inline float pL (R&& dir) const {
      return this->y * dir.x - this->x * dir.y; 
    }

    inline float modulo () const {
      return std::sqrt(this->x * this->x + this->y * this->y);
    }

    inline float modulo2 () const {
      return this->x * this->x + this->y * this->y;
    }

    inline float angle () const {
      return (this->x + this->y) * fisqrt(this->x * this->x + this->y * this->y);
    }
};

class AngDir2 {
  public:
    float x;
    float y;
    float a;
    
    /* Basic operations. */
    inline AngDir2 () noexcept: x(0.f), y(0.f), a(0.f) {}
    inline AngDir2 (float x, float y, float a) noexcept : x(x), y(y), a(a) {}
    inline AngDir2 (const AngDir2 & adir) noexcept: x(adir.x), y(adir.y), a(adir.a) {}
    inline AngDir2 (AngDir2 && adir) noexcept: x(adir.x), y(adir.y), a(adir.a) {}

    inline AngDir2 & operator= (AngDir2 && adir) noexcept {
      this->x = adir.x;
      this->y = adir.y;
      this->a = adir.a;
      return *this;
    }

    inline AngDir2 & operator= (const AngDir2 & adir) noexcept {
      this->x = adir.x;
      this->y = adir.y;
      this->a = adir.a;
      return *this;
    }

    /* Casting. */
    inline AngDir2 (const Dir2& dir) noexcept;
    inline AngDir2 (const Dir3& dir) noexcept;

    /* Comparations. */
    template<DirFin R>
    inline bool operator== (R&& adir) const {
      if constexpr (std::is_same_v<R, AngDir2>) 
        return this->x == adir.x && this->y == adir.y && this->a == adir.a;
      else 
        return this->x == adir.x && this->y == adir.y;
    }

    template<DirFin R>
    inline bool operator!= (R&& adir) const {
      if constexpr (std::is_same_v<R, AngDir2>)
        return this->x != adir.x || this->y != adir.y || this->a != adir.a;
      else
        return this->x != adir.x || this->y != adir.y;
    }

    /* Operators by overloading. */
    template<typename Self, DirFin R>
    inline auto operator+ (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x += dir.x;
        self.y += dir.y;
        if constexpr (std::is_same_v<std::remove_cvref_t<R>, AngDir2>)
          self.a += dir.a;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.x += self.x;
        dir.y += self.y;
        if constexpr (std::is_same_v<std::remove_cvref_t<R>, AngDir2>)
          dir.a += self.a;
        return std::forward<R>(dir);
      } else {
        if constexpr (std::is_same_v<std::remove_cvref_t<R>, AngDir2>)
          return AngDir2 {
            self.x + dir.x,
            self.y + dir.y,
            self.a + dir.a
          };
        else return AngDir2 {
          self.x + dir.x,
          self.y + dir.y,
          self.a
        };
      }
    }

    template<DirFin R> 
    inline AngDir2& operator+= (R&& d) {
      this->x += d.x;
      this->y += d.y;
      return *this;
    }

    template<DirFin R>
    inline float operator* (R&& dir) const {
      return this->x * dir.x + this->y * dir.y; 
    }

    template<typename Self>
    inline auto operator* (this Self&& self, const float& number) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x *= number;
        self.y *= number;
        return std::forward<Self>(self);
      } else return AngDir2 {
        self.x * number,
        self.y * number,
        self.a
      };
    }

    inline AngDir2& operator*= (const float& number) {
      this->x *= number;
      this->y *= number;
      return *this;
    }

    template<typename Self>
    inline auto operator- (this Self&& self) {
      if (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x = -self.x;
        self.y = -self.y;
        return std::forward<Self>(self);
      } else return AngDir2 {-self.x, -self.y, -self.a};
    }

    template<typename Self, DirFin R>
    inline auto operator- (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x -= dir.x;
        self.y -= dir.y;
        if constexpr (std::is_same_v<std::remove_cvref_t<R>, AngDir2>)
          self.a -= dir.a;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.x -= self.x;
        dir.y -= self.y;
        if constexpr (std::is_same_v<std::remove_cvref_t<R>, AngDir2>)
          self.a -= dir.a;
        return std::forward<R>(dir);
      } else {
        if constexpr (std::is_same_v<std::remove_cvref_t<R>, AngDir2>)
          return AngDir2 {
            self.x - dir.x,
            self.y - dir.y,
            self.a - dir.a
          };
        else return AngDir2 {
          self.x - dir.x,
          self.y - dir.y,
          self.a
        };
      }
    }

    template<DirFin R>
    inline AngDir2& operator-= (R&& dir) {
      this->x -= dir.x;
      this->y -= dir.y;
      return *this;
    }

    /* Operations by functions. */
    template<typename Self>
    inline auto percan (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>){
        std::swap(self.x, self.y);
        self.y = -self.y;
        return std::forward<Self>(self);
      } else return AngDir2 (-self.y, self.x, 0);
    }

    inline void rotate (const float& angle) {
      float sina = std::sin (angle);
      float cosa = std::cos (angle);
      float newx = this->x * cosa - this->y * sina;
      this->y = this->x * sina + this->y * cosa;
      this->x = newx;
    }

    template<typename Self>
    inline auto normalize (this Self&& self) {
      float inorm = fisqrt(self.x * self.x + self.y * self.y);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x *= inorm;
        self.y *= inorm;
        return std::forward<AngDir2>(self);
      } else return AngDir2 (
        self.x * inorm,
        self.y * inorm,
        self.a
      );
    }

    template<typename Self>
    inline auto abs (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && ! std::is_const_v<Self&&>) {
        absv(&self.x);
        absv(&self.y);
        return std::forward<Self&&>(self);
      } else return AngDir2 {
        absv (self.x),
        absv (self.y),
        self.a
      };
    }

    template<DirFin R>
    inline float pL (R&& dir) const {
      return this->y * dir.x - this->x * dir.y; 
    }

    inline float angle () const {
      return (this->x + this->y) * fisqrt(this->x * this->x + this->y * this->y);
    }

    inline float modulo () const {
      return std::sqrt(this->x * this->x + this->y * this->y);
    }

    inline float modulo2 () const {
      return this->x * this->x + this->y * this->y;
    }
};

class Dir3 {
  public:
    float x;
    float y;
    float z;

    inline Dir3 () noexcept: x(0.f), y(0.f), z(0.f) {}
    inline Dir3 (float x, float y, float z) noexcept: x(x), y(y), z(z) {}
    inline Dir3 (const Dir3 & dir) noexcept: x(dir.x), y(dir.y), z(dir.z) {}
    inline Dir3 (Dir3 && dir) noexcept: x(dir.x), y(dir.y), z(dir.z) {}

    inline Dir3& operator= (const Dir3 & dir) noexcept {
      this->x = dir.x;
      this->y = dir.y;
      this->z = dir.z;
      return *this;
    }

    inline Dir3& operator= (Dir3 && dir) noexcept {
      this->x = dir.x;
      this->y = dir.y;
      this->z = dir.z;
      return *this;
    }

    /* Casting. */
    inline Dir3 (const Dir2& dir) noexcept;
    inline Dir3 (const AngDir2& dir) noexcept;

    /* Comparations. */
    inline bool operator== (const Dir3& dir) const {
      return this->x == dir.x && this->y == dir.y && this->z == dir.z;
    }

    inline bool operator!= (const Dir3& dir) const {
      return this->x != dir.x || this->y != dir.y || this->z != dir.z;
    }

    /* Operators by overloading. */
    template<typename Self, DirFin3 R>
    inline auto operator+ (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x += dir.x;
        self.y += dir.y;
        self.z += dir.z;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.x += self.x;
        dir.y += self.y;
        dir.z += self.z;
        return std::forward<R>(dir);
      } else return Dir3 {
        self.x + dir.x,
        self.y + dir.y,
        self.z + dir.z
      };
    }

    template<DirFin3 R> 
    inline Dir3& operator+= (R&& d) {
      this->x += d.x;
      this->y += d.y;
      this->z += d.z;
      return *this;
    }

    template<DirFin3 R>
    inline float operator* (R&& dir) const {
      return this->x * dir.x + this->y * dir.y + this->z * dir.z; 
    }

    template<typename Self>
    inline auto operator* (this Self&& self, const float& number) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x *= number;
        self.y *= number;
        self.z *= number;
        return std::forward<Self>(self);
      } else return Dir3 {
        self.x * number,
        self.y * number,
        self.z * number
      };
    }

    inline Dir3& operator*= (const float& number) {
      this->x *= number;
      this->y *= number;
      this->z *= number;
      return *this;
    }

    template<typename Self>
    inline auto operator- (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x = -self.x;
        self.y = -self.y;
        self.z = -self.z;
        return std::forward<Self>(self);
      } else return Dir3 {-self.x, -self.y, -self.z};
    }

    template<typename Self, DirFin3 R>
    inline auto operator- (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x -= dir.x;
        self.y -= dir.y;
        self.z -= dir.z;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.x -= self.x;
        dir.y -= self.y;
        dir.z -= self.z;
        return std::forward<R>(dir);
      } else return Dir3 {
        self.x - dir.x,
        self.y - dir.y,
        self.z - dir.z
      };
    }

    template<DirFin3 R>
    inline Dir3& operator-= (R&& dir) {
      this->x -= dir.x;
      this->y -= dir.y;
      this->z -= dir.z;
      return *this;
    }

    /* Operations by functions. */

    template<typename Self>
    inline auto abs (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && ! std::is_const_v<Self&&>) {
        absv(&self.x);
        absv(&self.y);
        absv(&self.z);
        return std::forward<Self>(self);
      } else return Dir3 (
        absv (self.x),
        absv (self.y),
        absv (self.z)
      );
    }

    template<typename Self, DirFin3 R>
    inline auto cross (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        float sx = self.x, sy = self.y;
        self.x = self.y * dir.z - self.z * dir.y;
        self.y = self.z * dir.x - sx * dir.z;
        self.z = sx * dir.y - sy * dir.x;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        float dx = dir.x, dy = dir.y;
        dir.x = self.y * dir.z - self.z * dir.y;
        dir.y = self.z * dx - self.x * dir.z;
        dir.z = self.x * dy - self.y * dx;
        return std::forward<R>(dir);
      } else return Dir3 (
        self.y * dir.z - self.z * dir.y,
        self.z * dir.x - self.x * dir.z,
        self.x * dir.y - self.y * dir.x
      );
    }

    inline float modulo () const {
      return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    inline float modulo2 () const {
      return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    template<typename Self>
    inline auto normalize (this Self&& self) {
      float inorm = fisqrt(self.x * self.x + self.y * self.y + self.z * self.z);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.x *= inorm;
        self.y *= inorm;
        self.z *= inorm;
        return std::forward<Self> (self);
      } else return Dir3 {
        self.x * inorm,
        self.y * inorm,
        self.z * inorm
      };
    }

    inline void rotate_x (const float& angle) {
      float sina = std::sin (angle);
      float cosa = std::cos (angle);
      float newy = this->z * sina + this->y * cosa;
      this->z = this->z * cosa - this->y * sina;
      this->y = newy;
    }

    inline void rotate_y (const float& angle) {
      float sina = std::sin (angle);
      float cosa = std::cos (angle);
      float newx = this->x * cosa - this->z * sina;
      this->z = this->z * cosa + this->x * sina;
      this->x = newx;
    }

    inline void rotate_z (const float& angle) {
      float sina = std::sin (angle);
      float cosa = std::cos (angle);
      float newx = this->x * cosa + this->y * sina;
      this->y = this->y * cosa - this->x * sina;
      this->x = newx;
    }
};
    
Dir2::Dir2 (const Dir3& dir3) noexcept {
  this->x = dir3.x;
  this->y = dir3.y;
}

Dir2::Dir2 (const AngDir2& dir) noexcept {
  this->x = dir.x;
  this->y = dir.y;
}

AngDir2::AngDir2 (const Dir2& dir) noexcept {
  this->x = dir.x;
  this->y = dir.y;
}

AngDir2::AngDir2 (const Dir3& dir) noexcept {
  this->x = dir.x;
  this->y = dir.y;
}

Dir3::Dir3 (const Dir2& dir) noexcept {
  this->x = dir.x;
  this->y = dir.y;
  this->z = 0;
}

Dir3::Dir3 (const AngDir2& dir) noexcept {
  this->x = dir.x;
  this->y = dir.y;
  this->z = 0;
}
