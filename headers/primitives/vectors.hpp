#pragma once 

#include <SDL2/SDL_cpuinfo.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <smmintrin.h>
#include <type_traits>
#include <utility>
#include <cmath>
#include <xmmintrin.h>

/* This classes are used to represent vectors in the two and three dimentional space. 
 * */

class Dir2;
class AngDir2;
class Dir3;

template<class T>
concept DirFin = std::is_same_v<std::remove_cvref_t<T>, AngDir2> || std::is_same_v<std::remove_cvref_t<T>, Dir2>;

template<class T>
concept DirFin3 = std::is_same_v<std::remove_cvref_t<T>, Dir3>;

class alignas(16) Dir2 {
  public:
    union {
      struct { float x, y, pad1, pad2; };
      __m128 v;
    };

    /* Basic operations. */
    inline Dir2 () noexcept: v(_mm_setzero_ps()) {}
    inline Dir2 (__m128 m) noexcept: v(m) {}
    inline Dir2 (float x, float y) noexcept: x(x), y(y), pad1(0.f), pad2(0.f) {}
    inline Dir2 (const Dir2 & dir) noexcept: x(dir.x), y(dir.y), pad1(0.f), pad2(0.f) {}
    inline Dir2 (Dir2 && dir) noexcept: x(dir.x), y(dir.y), pad1(0.f), pad2(0.f) {}

    inline Dir2 & operator= (const Dir2 & dir) noexcept {
      this->v = dir.v;
      return *this;
    }

    /* Casting. */
    inline Dir2 (const Dir3& dir3) noexcept;
    inline Dir2 (const AngDir2& dir) noexcept;

    /* Comparations. */
    template<DirFin R>
    inline bool operator== (R&& dir) const {
      return (_mm_movemask_ps(_mm_cmpeq_ps (this->v, dir.v)) & 0b0011) == 0b0011;
    }

    template<DirFin R>
    inline bool operator!= (R&& dir) const {
      return (_mm_movemask_ps(_mm_cmpeq_ps (this->v, dir.v)) & 0b0011) != 0b0011;
    }

    /* Operators by overloading. */
    template<typename Self>
    inline auto operator- (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_xor_ps(self.v, _mm_set1_ps(-0.0f));
        return std::forward<Self>(self);
      } else return Dir2 {_mm_xor_ps(self.v, _mm_set1_ps(-0.0f))};
    }

    template<typename Self, DirFin R>
    inline auto operator- (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_sub_ps (self.v, dir.v);
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = _mm_sub_ps (self.v, dir.v);
        return std::forward<R>(dir);
      } else return Dir2 {_mm_sub_ps (self.v, dir.v)};
    }

    template<DirFin R>
    inline Dir2& operator-= (R&& dir) {
      this->v = _mm_sub_ps(this->v, dir.v);
      return *this;
    }
    
    template<typename Self, DirFin R>
    inline auto operator+ (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_add_ps (self.v, dir.v);
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = _mm_add_ps (self.v, dir.v);
        return std::forward<R>(dir);
      } else return Dir2 {_mm_add_ps (self.v, dir.v)};
    }

    template<DirFin R> 
    inline Dir2& operator+= (R&& dir) {
      this->v = _mm_add_ps (this->v, dir.v);
      return *this;
    }
    
    template<DirFin R>
    inline float operator* (R&& dir) const {
      __m128 res = _mm_mul_ps (this->v, dir.v);
      return _mm_cvtss_f32 (_mm_add_ps (res, _mm_shuffle_ps(res, res, 0b01010101)));
    }

    template<typename Self>
    inline auto operator* (this Self&& self, const float& number) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_mul_ps(self.v, _mm_set1_ps (number));
        return std::forward<Self>(self);
      } else return Dir2 {_mm_mul_ps(self.v, _mm_set1_ps (number))};
    }

    template<typename Self>
    inline auto operator/ (this Self&& self, const float& number) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_mul_ps(self.v, _mm_rcp_ps(_mm_set1_ps (number)));
        return std::forward<Self>(self);
      } else return Dir2 {_mm_mul_ps(self.v, _mm_rcp_ps(_mm_set1_ps (number)))};
    }

    inline Dir2& operator*= (const float& number) {
      this->v = _mm_mul_ps(this->v, _mm_set1_ps(number));
      return *this;
    }

    /* Operations by functions. */
    template<typename Self>
    inline auto percan (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>){
        self.v = _mm_xor_ps (
          _mm_shuffle_ps(self.v, self.v, 0b11100001),
          _mm_set_ps (0.f, -0.f, 0.f, -0.f)
        );
        return std::forward<Self>(self);
      } else return Dir2 {_mm_xor_ps (
        _mm_shuffle_ps(self.v, self.v, 0b11100001),
        _mm_set_ps (0.f, -0.f, 0.f, -0.f)
      )};
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
      __m128 opr = _mm_mul_ps (self.v, self.v);
      opr = _mm_mul_ps (_mm_set1_ps(_mm_cvtss_f32(_mm_rsqrt_ss(_mm_hadd_ps(opr, opr)))), self.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 {opr};
    }

    template<typename Self>
    inline auto abs (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_and_ps(self.v, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
        return std::forward<Self>(self);
      } else return Dir2 {_mm_and_ps(self.v, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)))};
    }

    template<DirFin R>
    inline float pL (R&& dir) const {
      __m128 opr = _mm_mul_ps (_mm_shuffle_ps(this->v, this->v, 0b11100001), dir.v);
      return _mm_cvtss_f32 (_mm_hsub_ps(opr, opr));
    }

    template<DirFin R1, DirFin R2>
    inline float pLd (R1&& dir1, R2&& dir2) const {
      __m128 mine = _mm_shuffle_ps(this->v, this->v, 0b00010001);
      __m128 opr1 = _mm_mul_ps (mine, dir1.v);
      __m128 opr2 = _mm_mul_ps (mine, dir2.v);
      __m128 opr = _mm_hsub_ps(opr1, opr2);
      return _mm_cvtss_f32 (_mm_div_ps(opr, _mm_shuffle_ps(opr, opr, 0b00010010)));
    }

    inline float modulo () const {
      __m128 opr = _mm_mul_ps(this->v, this->v);
      return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(opr, opr)));
    }

    inline float sum () const {
      return _mm_cvtss_f32(_mm_hadd_ps(this->v, this->v));
    }

    inline float modulo2 () const {
      __m128 opr = _mm_mul_ps(this->v, this->v);
      return _mm_cvtss_f32(_mm_hadd_ps(opr, opr));
    }

    inline float angle () const {
      __m128 opr = _mm_mul_ps(this->v, this->v);
      return 
        _mm_cvtss_f32 (_mm_rsqrt_ss(_mm_hadd_ps(opr, opr))) *
        _mm_cvtss_f32(_mm_hadd_ps(this->v, this->v));
    }

    template<typename Self>
    inline auto max0 (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_max_ps (self.v, _mm_set1_ps(0.f));
        return std::forward<Self>(self);
      } else return Dir2 {_mm_max_ps (self.v, _mm_set1_ps(0.f))};
    }

    template<typename Self, DirFin R>
    inline auto bound (this Self&& self, R&& dir) {
      __m128 opr = _mm_max_ps (_mm_min_ps (self.v, dir.v), _mm_or_ps(dir.v, _mm_set1_ps(-0.0f)));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 {opr};
    }

    template<typename Self>
    inline auto bound01 (this Self&& self) {
      __m128 opr = _mm_max_ps (_mm_min_ps (self.v, _mm_set1_ps(1.f)), _mm_set1_ps(0.f));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 {opr};
    }

    template<typename Self, DirFin R>
    inline auto dir_mul (this Self&& self, R&& dir) {
      __m128 opr = _mm_mul_ps (self.v, dir.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 {opr};
    }

    template<typename Self, DirFin R>
    inline auto madd (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fmadd_ps (self.v, _mm_set1_ps (coef), dir.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 {opr};
    }

    template<typename Self, DirFin R>
    inline auto msub (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fmsub_ps (self.v, _mm_set1_ps (coef), dir.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 {opr};
    }

    template<typename Self, DirFin R>
    inline auto nmadd (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fnmadd_ps (self.v, _mm_set1_ps (coef), dir.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 {opr};
    }

    template<typename Self, DirFin R>
    inline auto nmsub (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fnmsub_ps (self.v, _mm_set1_ps (coef), dir.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 {opr};
    }

    template<typename Self>
    inline auto round (this Self&& self) {
      __m128 opr = _mm_round_ps (self.v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 {opr};
    }
};



class alignas(16) AngDir2 {
  public:
    union {
      struct {float x, y, a, pad;};
      __m128 v;
    };
    
    /* Basic operations. */
    inline AngDir2 () noexcept  {
      this->v = _mm_setzero_ps();
    }

    inline AngDir2 (float coef) noexcept {
      this->v = _mm_set_ps(0.f, coef, coef, coef);
    }
    
    inline AngDir2 (__m128 m) noexcept {
      this->v = m;
    }

    inline AngDir2 (float x, float y, float a) noexcept {
      this->v = _mm_set_ps(0.f, a, y, x);
    }

    inline AngDir2 (const AngDir2 & adir) noexcept {
      this->v = _mm_set_ps(0.f, adir.a, adir.y, adir.x);
    }

    inline AngDir2 & operator= (const AngDir2 & adir) noexcept {
      this->v = adir.v;
      return *this;
    }

    /* Casting. */
    inline AngDir2 (const Dir2& dir) noexcept;
    inline AngDir2 (const Dir3& dir) noexcept;

    /* Comparations. */
    template<DirFin R>
    inline bool operator== (R&& adir) const {
      if constexpr (std::is_same_v<R, AngDir2>) 
        return _mm_movemask_ps(_mm_cmpeq_ps (this->v, adir.v)) == 0b1111;
      else 
        return (_mm_movemask_ps(_mm_cmpeq_ps (this->v, adir.v)) & 0b1100) == 0b1111;
    }

    template<DirFin R>
    inline bool operator!= (R&& adir) const {
      if constexpr (std::is_same_v<R, AngDir2>) 
        return _mm_movemask_ps(_mm_cmpeq_ps (this->v, adir.v)) != 0b1111;
      else 
        return (_mm_movemask_ps(_mm_cmpeq_ps (this->v, adir.v)) & 0b1100) != 0b1111;
    }

    /* Operators by overloading. */
    template<typename Self, DirFin R>
    inline auto operator+ (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_add_ps (self.v, dir.v);
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = _mm_add_ps (self.v, dir.v);
        return std::forward<R>(dir);
      } else return AngDir2 {_mm_add_ps (self.v, dir.v)};
    }

    template<DirFin R> 
    inline AngDir2& operator+= (R&& dir) {
      this->v = _mm_add_ps (this->v, dir.v);
      return *this;
    }

    template<DirFin R>
    inline float operator* (R&& dir) const {
      __m128 res = _mm_mul_ps(this->v, dir.v);
      return _mm_cvtss_f32(_mm_hadd_ps (res, res));
    }

    template<typename Self>
    inline auto operator* (this Self&& self, const float& number) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_mul_ps(self.v, _mm_set_ps (0.f, 1.f, number, number));
        return std::forward<Self>(self);
      } else return AngDir2 {_mm_mul_ps(self.v, _mm_set_ps (0.f, 1.f, number, number))};
    }

    template<typename Self>
    inline auto operator/ (this Self&& self, const float& number) {
      __m128 opr = _mm_mul_ps(self.v, _mm_rcp_ps(_mm_set_ps (1.f, 1.f, number, number)));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return AngDir2 {opr};
    }

    inline AngDir2& operator*= (const float& number) {
      this->v = _mm_mul_ps (this->v, _mm_set_ps (0.f, 1.f, number, number));
      return *this;
    }

    template<typename Self>
    inline auto operator- (this Self&& self) {
      if (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_xor_ps(self.v, _mm_set1_ps(-0.f));
        return std::forward<Self>(self);
      } else return AngDir2 {_mm_xor_ps(self.v, _mm_set1_ps(-0.f))};
    }

    template<typename Self, DirFin R>
    inline auto operator- (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_sub_ps (self.v, dir.v);
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = _mm_sub_ps (self.v, dir.v);
        return std::forward<R>(dir);
      } else return AngDir2 {_mm_sub_ps (self.v, dir.v)};
    }

    template<DirFin R>
    inline AngDir2& operator-= (R&& dir) {
      this->v = _mm_sub_ps (this->v, dir.v);
      return *this;
    }

    /* Operations by functions. */
    template<typename Self>
    inline auto percan (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>){
        self.v = _mm_castsi128_ps(_mm_xor_si128 (
          _mm_shuffle_epi32(_mm_castps_si128(self.v), 0b11100001),
          _mm_set_epi64x(0, 0x80000000)
        ));
        return std::forward<Self>(self);
      } else return AngDir2 {_mm_castsi128_ps(_mm_xor_si128 (
        _mm_shuffle_epi32(_mm_castps_si128(self.v), 0b11100001),
        _mm_set_epi64x(0, 0x80000000)
      ))};
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
      __m128 opr = _mm_mul_ps (self.v, self.v);
      float b = _mm_cvtss_f32 (_mm_rsqrt_ss (_mm_hadd_ps (opr, opr)));
      opr = _mm_mul_ps (_mm_set_ps (0.f, 1.f, b, b), self.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<AngDir2>(self);
      } else return AngDir2 {opr};
    }

    template<typename Self>
    inline auto abs (this Self&& self) {
      __m128 opr = _mm_and_ps(self.v, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self&&>(self);
      } else return AngDir2 {opr};
    }

    template<DirFin R>
    inline float pL (R&& dir) const {
      __m128 opr = _mm_mul_ps (
        _mm_castsi128_ps (_mm_shuffle_epi32 (_mm_castps_si128 (this->v), 0b11100001)),
        dir.v
      );
      return _mm_cvtss_f32 (_mm_hsub_ps(opr, opr));
    }

    inline float modulo () const {
      __m128 opr = _mm_mul_ps(this->v, this->v);
      return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(opr, opr)));
    }

    inline float modulo2 () const {
      __m128 opr = _mm_mul_ps(this->v, this->v);
      return _mm_cvtss_f32(_mm_hadd_ps(opr, opr));
    }

    inline float angle () const {
      __m128 opr = _mm_mul_ps(this->v, this->v);
      return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_hadd_ps(opr, opr))) * _mm_cvtss_f32(_mm_hadd_ps(this->v, this->v));
    }

    template<typename Self>
    inline auto max0 (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_max_ps (self.v, _mm_set1_ps(0.f));
        return std::forward<Self>(self);
      } else return AngDir2 {_mm_max_ps (self.v, _mm_set1_ps(0.f))};
    }

    template<typename Self, DirFin R>
    inline auto bound (this Self&& self, R&& dir) {
      __m128 opr = _mm_max_ps (_mm_min_ps (self.v, dir.v), _mm_or_ps(dir.v, _mm_set1_ps(-0.0f)));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return AngDir2 {opr};
    }

    template<typename Self>
    inline auto bound01 (this Self&& self) {
      __m128 opr = _mm_max_ps (_mm_min_ps (self.v, _mm_set1_ps(1.f)), _mm_set1_ps(0.f));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return AngDir2 {opr};
    }

    template<typename Self, DirFin R>
    inline auto dir_mul (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_mul_ps (self.v, dir.v);
        return std::forward<Self>(self);
      } else return AngDir2 {_mm_mul_ps (self.v, dir.v)};
    }

    template<typename Self, DirFin R>
    inline auto madd (this Self&& self, const float& coef, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_fmadd_ps (self.v, _mm_set1_ps (coef), dir.v);
        return std::forward<Self>(self);
      } else return AngDir2 {_mm_fmadd_ps (self.v, _mm_set1_ps (coef), dir.v)};
    }

    template<typename Self, DirFin R>
    inline auto msub (this Self&& self, const float& coef, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_fmsub_ps (self.v, _mm_set1_ps (coef), dir.v);
        return std::forward<Self>(self);
      } else return AngDir2 {_mm_fmsub_ps (self.v, _mm_set1_ps (coef), dir.v)};
    }

    template<typename Self, DirFin R>
    inline auto nmadd (this Self&& self, const float& coef, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_fnmadd_ps (self.v, _mm_set1_ps (coef), dir.v);
        return std::forward<Self>(self);
      } else return AngDir2 {_mm_fnmadd_ps (self.v, _mm_set1_ps (coef), dir.v)};
    }

    template<typename Self, DirFin R>
    inline auto nmsub (this Self&& self, const float& coef, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_fnmsub_ps (self.v, _mm_set1_ps (coef), dir.v);
        return std::forward<Self>(self);
      } else return AngDir2 {_mm_fnmsub_ps (self.v, _mm_set1_ps (coef), dir.v)};
    }
};



class alignas(16) Dir3 {
  public:
    union {
      struct {float x, y, z, pad;};
      __m128 v;
    };

    inline Dir3 () noexcept: v(_mm_setzero_ps()) {}
    inline Dir3 (__m128 m) noexcept: v(m) {}
    inline Dir3 (float x, float y, float z) noexcept: x(x), y(y), z(z), pad(0.f) {}
    inline Dir3 (const Dir3 & dir) noexcept: x(dir.x), y(dir.y), z(dir.z), pad(0.f) {}
    inline Dir3 (Dir3 && dir) noexcept: x(dir.x), y(dir.y), z(dir.z), pad(0.f) {}

    inline Dir3& operator= (const Dir3 & dir) noexcept {
      this->v = dir.v;
      return *this;
    }

    inline Dir3& operator= (Dir3 && dir) noexcept {
      this->v = dir.v;
      return *this;
    }

    /* Casting. */
    inline Dir3 (const Dir2& dir) noexcept;
    inline Dir3 (const AngDir2& dir) noexcept;

    /* Comparations. */
    inline bool operator== (const Dir3& dir) const {
      return _mm_movemask_ps(_mm_cmpeq_ps (this->v, dir.v)) == 0b1111;
    }

    inline bool operator!= (const Dir3& dir) const {
      return _mm_movemask_ps(_mm_cmpeq_ps (this->v, dir.v)) != 0b1111;
    }

    /* Operators by overloading. */
    template<typename Self, DirFin3 R>
    inline auto operator+ (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps (&self.x, _mm_add_ps (_mm_load_ps (&self.x), _mm_load_ps (&dir.x)));
        self.v = _mm_add_ps (dir.v, self.v);
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = _mm_add_ps (dir.v, self.v);
        return std::forward<R>(dir);
      } else return Dir3 {_mm_add_ps (dir.v, self.v)};
    }

    template<DirFin3 R> 
    inline Dir3& operator+= (R&& dir) {
      this->v = _mm_add_ps (this->v, dir.v);
      return *this;
    }

    template<DirFin3 R>
    inline float operator* (R&& dir) const {
      __m128 opr = _mm_mul_ps(this->v, dir.v);
      opr = _mm_hadd_ps(opr, opr);
      return _mm_cvtss_f32(_mm_hadd_ps(opr, opr));
    }

    template<typename Self>
    inline auto operator* (this Self&& self, const float& number) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_mul_ps (self.v, _mm_set1_ps(number));
        return std::forward<Self>(self);
      } else return Dir3 {_mm_mul_ps (self.v, _mm_set1_ps(number))};
    }

    inline Dir3& operator*= (const float& number) {
      this->v = _mm_mul_ps (this->v, _mm_set1_ps(number));
      return *this;
    }

    template<typename Self>
    inline auto operator- (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_xor_ps (self.x, _mm_set1_ps(-0.f));
        return std::forward<Self>(self);
      } else return Dir3 {_mm_xor_ps (self.x, _mm_set1_ps(-0.f))};
    }

    template<typename Self, DirFin3 R>
    inline auto operator- (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_sub_ps (self.v, dir.v);
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = _mm_sub_ps (self.v, dir.v);
        return std::forward<R>(dir);
      } else return Dir3 {_mm_sub_ps (self.v, dir.v)};
    }

    template<DirFin3 R>
    inline Dir3& operator-= (R&& dir) {
      this->v = _mm_sub_ps (this->v, dir.v);
      return *this;
    }

    /* Operations by functions. */

    template<typename Self>
    inline auto abs (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && ! std::is_const_v<Self&&>) {
        self.v = _mm_and_ps (self.v, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
        return std::forward<Self>(self);
      } else return Dir3 {_mm_and_ps (self.v, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)))};
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
      __m128 opr = _mm_mul_ps(this->v, this->v);
      opr = _mm_hadd_ps(opr, opr);
      return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(opr, opr)));
    }

    inline float modulo2 () const {
      __m128 opr = _mm_mul_ps(this->v, this->v);
      opr = _mm_hadd_ps(opr, opr);
      return _mm_cvtss_f32(_mm_hadd_ps(opr, opr));
    }

    template<typename Self>
    inline auto normalize (this Self&& self) {
      __m128 opr = _mm_mul_ps (self.v, self.v);
      opr = _mm_hadd_ps(opr, opr);
      float b = _mm_cvtss_f32(_mm_rsqrt_ss(_mm_hadd_ps(opr, opr)));
      opr = _mm_mul_ps(_mm_set1_ps (b), self.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self> (self);
      } else return Dir3 {opr};
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
  this->v = dir3.v;
}

Dir2::Dir2 (const AngDir2& dir) noexcept {
  this->v = dir.v;
}

AngDir2::AngDir2 (const Dir2& dir) noexcept {
  this->x = dir.x;
  this->y = dir.y;
  this->a = 0.f;
  this->pad = 0.f;
}

AngDir2::AngDir2 (const Dir3& dir) noexcept {
  this->x = dir.x;
  this->y = dir.y;
  this->a = 0.f;
  this->pad = 0.f;
}

Dir3::Dir3 (const Dir2& dir) noexcept {
  this->x = dir.x;
  this->y = dir.y;
  this->z = 0.f;
  this->pad = 0.f;
}

Dir3::Dir3 (const AngDir2& dir) noexcept {
  this->x = dir.x;
  this->y = dir.y;
  this->z = 0.f;
  this->pad = 0.f;
}
