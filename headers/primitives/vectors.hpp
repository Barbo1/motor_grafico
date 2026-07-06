#pragma once 

#include <emmintrin.h>
#include <immintrin.h>
#include <smmintrin.h>
#include <xmmintrin.h>
#include <SDL2/SDL_cpuinfo.h>
#include <type_traits>
#include <bit>
#include <utility>
#include <array>
#include <cmath>

/* This classes are used to represent vectors in the two and three dimentional space. 
 * */

class Dir2;
class AngDir2;
class Dir3;

template<class T>
concept DirFin = std::is_same_v<std::remove_cvref_t<T>, AngDir2> || std::is_same_v<std::remove_cvref_t<T>, Dir2>;

class MemDir2 {
  public:
    float x, y;

    inline MemDir2() noexcept: x(0.f), y(0.f) {}
    inline MemDir2(float x, float y) noexcept: x(x), y(y) {}
    inline MemDir2(const MemDir2& dir) noexcept: x(dir.x), y(dir.y) {}
    inline MemDir2& operator=(const MemDir2& dir) { 
      this->x = dir.x; 
      this->y = dir.y; 
      return *this; 
    }

    template<DirFin T>
    inline MemDir2& store(T&& dir);
};

class alignas(16) Dir2 {
  public:
    __m128 v;

    /* Basic operations. */
    inline Dir2 () noexcept: v(_mm_setzero_ps()) {}
    inline Dir2 (__m128 m) noexcept { this->v = _mm_shuffle_ps(m, m, 0b01000100); }
    inline Dir2 (float x, float y) noexcept{ this->v = _mm_set_ps(y, x, y, x); }
    inline Dir2 (const Dir2 & dir) noexcept: v(dir.v) {}
    inline Dir2 (Dir2 && dir) noexcept: v(dir.v) {}
    static inline Dir2 from_well(__m128 m) {
      Dir2 ret;
      ret.v = m;
      return ret;
    }

    inline Dir2 & operator= (const Dir2 & dir) noexcept {
      this->v = dir.v;
      return *this;
    }

    /* Casting. */
    inline Dir2 (const MemDir2& dir) noexcept;
    inline Dir2 (const Dir3& dir3) noexcept;
    inline Dir2 (const AngDir2& dir) noexcept;

    /* Comparations. */
    inline bool operator== (const Dir2& dir) const {
      return _mm_movemask_ps(_mm_cmpeq_ps (this->v, dir.v)) == 0b1111;
    }

    inline bool operator!= (const Dir2& dir) const {
      return _mm_movemask_ps(_mm_cmpeq_ps (this->v, dir.v)) != 0b1111;
    }
    
    inline bool operator> (const Dir2& dir) const {
      return _mm_movemask_ps(_mm_cmpgt_ps (this->v, dir.v)) == 0b1111;
    }
    
    inline bool operator< (const Dir2& dir) const {
      return _mm_movemask_ps(_mm_cmplt_ps (this->v, dir.v)) == 0b1111;
    }
    
    inline bool operator>= (const Dir2& dir) const {
      return _mm_movemask_ps(_mm_cmpge_ps (this->v, dir.v)) == 0b1111;
    }
    
    inline bool operator<= (const Dir2& dir) const {
      return _mm_movemask_ps(_mm_cmple_ps (this->v, dir.v)) == 0b1111;
    }
    
    inline bool operator> (float bound) const {
      return _mm_movemask_ps(_mm_cmpgt_ps (this->v, _mm_set1_ps(bound))) == 0b1111;
    }
    
    inline bool operator< (float bound) const {
      return _mm_movemask_ps(_mm_cmplt_ps (this->v, _mm_set1_ps(bound))) == 0b1111;
    }
    
    inline bool operator>= (float bound) const {
      return _mm_movemask_ps(_mm_cmpge_ps (this->v, _mm_set1_ps(bound))) == 0b1111;
    }
    
    inline bool operator<= (float bound) const {
      return _mm_movemask_ps(_mm_cmple_ps (this->v, _mm_set1_ps(bound))) == 0b1111;
    }

    /* Obtaining components. */
    inline float y() const {
      return std::bit_cast<float>(_mm_extract_ps(this->v, 1));
    }
    
    inline float x() const {
      return _mm_cvtss_f32(this->v);
    }
    
    inline void y(float y) {
      __m128 y_mm = _mm_set1_ps(y);
      this->v = _mm_blend_ps(this->v, y_mm, 0b1010);
    }
    
    inline void x(float x) {
      __m128 x_mm = _mm_set1_ps(x);
      this->v = _mm_blend_ps(this->v, x_mm, 0b0101);
    }

    inline Dir2& turn() {
      this->v = _mm_shuffle_ps(this->v, this->v, 0b00010001);
      return *this;
    }

    /* Operators by overloading. */
    template<typename Self>
    inline auto operator- (this Self&& self) {
      __m128 opr = _mm_xor_ps(self.v, _mm_set1_ps(-0.0f));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 (opr);
    }

    template<typename Self, typename R>
    requires std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>
    inline auto operator- (this Self&& self, R&& dir) {
      __m128 opr = _mm_sub_ps (self.v, dir.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = opr;
        return std::forward<R>(dir);
      } else return Dir2 (opr);
    }

    inline Dir2& operator-= (const Dir2& dir) {
      this->v = _mm_sub_ps(this->v, dir.v);
      return *this;
    }
    
    template<typename Self, typename R>
    requires std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>
    inline auto operator+ (this Self&& self, R&& dir) {
      __m128 opr = _mm_add_ps (self.v, dir.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = opr;
        return std::forward<R>(dir);
      } else return Dir2 (opr);
    }

    inline Dir2& operator+= (Dir2 const& dir) {
      this->v = _mm_add_ps (this->v, dir.v);
      return *this;
    }
    
    inline float operator* (const Dir2& dir) const {
      __m128 res = _mm_mul_ps (this->v, dir.v);
      return _mm_cvtss_f32 (_mm_add_ps (res, _mm_shuffle_ps(res, res, 0b01010101)));
    }

    template<typename Self>
    inline auto operator* (this Self&& self, const float& number) {
      __m128 opr = _mm_mul_ps(self.v, _mm_set1_ps (number));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 (opr);
    }

    template<typename Self>
    inline auto operator/ (this Self&& self, const float& number) {
      __m128 opr = _mm_div_ps(self.v, _mm_set1_ps (number));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 (opr);
    }

    inline Dir2& operator*= (const float& number) {
      this->v = _mm_mul_ps(this->v, _mm_set1_ps(number));
      return *this;
    }

    /* Operations by functions. */
    template<typename Self>
    inline auto percan (this Self&& self) {
      __m128 opr = _mm_xor_ps (
        _mm_shuffle_ps(self.v, self.v, 0b00010001),
        _mm_set_ps(0.f, -0.f, 0.f, -0.f)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>){
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 (opr);
    }

    template<typename Self>
    inline auto normalize (this Self&& self) {
      __m128 op1 = _mm_mul_ps(self.v, self.v);
      __m128 op2 = _mm_hadd_ps(op1, op1);
      __m128 op3 = _mm_rsqrt_ps(op2);
      __m128 opr1 = _mm_mul_ps(op3, self.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr1;
        return std::forward<Self>(self);
      } else return Dir2 (opr1);
    }

    template<typename Self>
    inline auto abs (this Self&& self) {
      __m128 opr = _mm_andnot_ps(_mm_set1_ps(-0.f), self.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 (opr);
    }

    inline float pL (const Dir2& dir) const {
      __m128 opr = _mm_mul_ps(_mm_shuffle_ps(this->v, _mm_undefined_ps(), 0b00010001), dir.v);
      return _mm_cvtss_f32(_mm_hsub_ps(opr, opr));
    }

    template<typename R1, typename R2>
    requires std::is_same_v<std::remove_cvref_t<R1>, std::remove_cvref_t<R2>>
    inline float pLd (R1&& dir1, R2&& dir2) const {
      __m128 mine = _mm_shuffle_ps(this->v, this->v, 0b00010001);
      __m128 other = _mm_movelh_ps(dir1.v, dir2.v);
      __m128 res1 = _mm_mul_ps(mine, other);
      __m128 opr = _mm_hsub_ps(res1, _mm_undefined_ps());
      return _mm_cvtss_f32(_mm_div_ss(opr, _mm_shuffle_ps(opr, opr, 1)));
    }

    inline float modulo () const {
      __m128 op = _mm_mul_ps(this->v, this->v);
      return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(op, op)));
    }

    inline float modulo2 () const {
      __m128 op = _mm_mul_ps(this->v, this->v);
      return _mm_cvtss_f32(_mm_hadd_ps(op, op));
    }

    inline float sum () const {
      return _mm_cvtss_f32(_mm_hadd_ps(this->v, this->v));
    }

    template<typename Self>
    inline auto max0 (this Self&& self) {
      __m128 opr = _mm_andnot_ps(_mm_castsi128_ps(_mm_srai_epi32(_mm_castps_si128(self.v), 31)), self.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 (opr);
    }

    template<typename Self, typename R>
    requires std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>
    inline auto bound (this Self&& self, R&& dir) {
      __m128 op = _mm_min_ps(self.v, dir.v);
      __m128 opr = _mm_max_ps(op, _mm_xor_ps(dir.v, _mm_set1_ps(-0.f)));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = opr;
        return std::forward<R>(dir);
      } else return Dir2 (opr);
    }

    template<typename Self>
    inline auto bound01 (this Self&& self) {
      __m128i casted = _mm_castps_si128(self.v);
      __m128i opr = _mm_andnot_si128(_mm_srai_epi32(casted, 31), casted);
      __m128 opr1 = _mm_castsi128_ps(_mm_min_epi32 (opr, _mm_set1_epi32(0x3f800000)));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr1;
        return std::forward<Self>(self);
      } else return Dir2 (opr1);
    }

    template<typename Self, typename R>
    requires std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>
    inline auto dir_mul (this Self&& self, R&& dir) {
      __m128 opr = _mm_mul_ps (self.v, dir.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = opr;
        return std::forward<R>(dir);
      } else return Dir2 (opr);
    }

    template<typename Self, typename R>
    requires std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>
    inline auto madd (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fmadd_ps (self.v, _mm_set1_ps (coef), dir.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = opr;
        return std::forward<R>(dir);
      } else return Dir2 (opr);
    }

    template<typename Self, typename R>
    requires std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>
    inline auto msub (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fmsub_ps (self.v, _mm_set1_ps (coef), dir.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = opr;
        return std::forward<R>(dir);
      } else return Dir2 (opr);
    }

    template<typename Self, typename R>
    requires std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>
    inline auto nmadd (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fnmadd_ps (self.v, _mm_set1_ps (coef), dir.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = opr;
        return std::forward<R>(dir);
      } else return Dir2 (opr);
    }

    template<typename Self, typename R>
    requires std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>
    inline auto nmsub (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fnmsub_ps (self.v, _mm_set1_ps (coef), dir.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = opr;
        return std::forward<R>(dir);
      } else return Dir2 (opr);
    }

    template<typename Self>
    inline auto round (this Self&& self) {
      __m128 opr = _mm_round_ps (self.v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir2 (opr);
    }
};



class alignas(16) AngDir2 {
  public:
    __m128 v;
    
    /* Basic operations. */
    inline AngDir2 () noexcept: v(_mm_setzero_ps())  {}
    inline AngDir2 (float coef) noexcept: v(_mm_set1_ps(coef)) {}
    inline AngDir2 (__m128 m) noexcept: v(m) {}
    inline AngDir2 (float x, float y, float a) noexcept: v(_mm_set_ps(0.f, a, y, x)) {}
    inline AngDir2 (const AngDir2 & adir) noexcept: v(adir.v) {}

    inline AngDir2 & operator= (const AngDir2 & adir) noexcept {
      this->v = adir.v;
      return *this;
    }

    /* Casting. */
    inline AngDir2 (const MemDir2& dir) noexcept;
    inline AngDir2 (const Dir2& dir) noexcept;
    inline AngDir2 (const Dir2& dir, const float& angle) noexcept;
    inline AngDir2 (const Dir3& dir) noexcept;

    /* Comparations. */
    template<DirFin R>
    inline bool operator== (R&& adir) const {
      if constexpr (std::is_same_v<R, AngDir2>) 
        return _mm_movemask_ps(_mm_cmpeq_ps (this->v, adir.v)) == 0b1111;
      else 
        return (_mm_movemask_ps(_mm_cmpeq_ps (this->v, adir.v)) & 0b1100) == 0b1100;
    }

    template<DirFin R>
    inline bool operator!= (R&& adir) const {
      if constexpr (std::is_same_v<R, AngDir2>) 
        return _mm_movemask_ps(_mm_cmpeq_ps (this->v, adir.v)) != 0b1111;
      else 
        return (_mm_movemask_ps(_mm_cmpeq_ps (this->v, adir.v)) & 0b1100) != 0b1100;
    }

    /* Obtaining components. */
    inline float a() const {
      return std::bit_cast<float>(_mm_extract_ps(this->v, 2));
    }

    inline float y() const {
      return std::bit_cast<float>(_mm_extract_ps(this->v, 1));
    }
    
    inline float x() const {
      return _mm_cvtss_f32(this->v);
    }

    inline void a(float a) {
      __m128 a_mm = _mm_set1_ps(a);
      this->v = _mm_blend_ps(this->v, a_mm, 0b0100);
    }

    inline void y(float y) {
      __m128 y_mm = _mm_set1_ps(y);
      this->v = _mm_blend_ps(this->v, y_mm, 0b0010);
    }

    inline void x(float x) {
      __m128 x_mm = _mm_set1_ps(x);
      this->v = _mm_move_ss(this->v, x_mm);
    }

    /* Operators by overloading. */
    template<typename Self, DirFin R>
    inline auto operator+ (this Self&& self, R&& dir) {
      __m128 opr;
      if constexpr (std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>)
        opr = _mm_add_ps (self.v, dir.v);
      else
        opr = _mm_add_ps (self.v, _mm_movelh_ps(dir.v, _mm_setzero_ps()));

      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else if constexpr (
          std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>> &&
          std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = opr;
        return std::forward<R>(dir);
      } else return AngDir2 (opr);
    }

    template<DirFin R> 
    inline AngDir2& operator+= (R&& dir) {
      if constexpr (std::is_same_v<std::remove_cvref_t<R>, AngDir2>)
        this->v = _mm_add_ps (this->v, _mm_movelh_ps(dir.v, _mm_setzero_ps()));
      else
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
        self.v = _mm_mul_ps(self.v, _mm_set1_ps (number));
        return std::forward<Self>(self);
      } else return AngDir2 (_mm_mul_ps(self.v, _mm_set1_ps (number)));
    }

    template<typename Self>
    inline auto operator/ (this Self&& self, const float& number) {
      __m128 opr = _mm_mul_ps(self.v, _mm_rcp_ps(_mm_set1_ps (number)));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return AngDir2 (opr);
    }

    inline AngDir2& operator*= (const float& number) {
      this->v = _mm_mul_ps (this->v, _mm_set1_ps (number));
      return *this;
    }

    template<typename Self>
    inline auto operator- (this Self&& self) {
      if (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_xor_ps(self.v, _mm_set1_ps(-0.f));
        return std::forward<Self>(self);
      } else return AngDir2 (_mm_xor_ps(self.v, _mm_set1_ps(-0.f)));
    }

    template<typename Self, DirFin R>
    inline auto operator- (this Self&& self, R&& dir) {
      __m128 opr;
      if constexpr (std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>)
        opr = _mm_sub_ps (self.v, dir.v);
      else
        opr = _mm_sub_ps (self.v, _mm_movelh_ps(dir.v, _mm_setzero_ps()));

      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else if constexpr (
          std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>> &&
          std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = opr;
        return std::forward<R>(dir);
      } else return AngDir2 (opr);
    }

    template<DirFin R> 
    inline AngDir2& operator-= (R&& dir) {
      if constexpr (std::is_same_v<std::remove_cvref_t<R>, AngDir2>)
        this->v = _mm_sub_ps (this->v, _mm_movelh_ps(dir.v, _mm_setzero_ps()));
      else
        this->v = _mm_sub_ps (this->v, dir.v);
      return *this;
    }

    /* Operations by functions. */
    template<typename Self>
    inline auto percan (this Self&& self) {
      __m128 op = _mm_xor_ps (
        _mm_shuffle_ps(self.v, self.v, 0b11100001),
        _mm_set_ps(0.f, 0.f, 0.f, -0.f)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>){
        self.v = op;
        return std::forward<Self>(self);
      } else return AngDir2 (op);
    }

    template<typename Self>
    inline auto normalize (this Self&& self) {
      __m128 op1 = _mm_movelh_ps(self.v, self.v);
      __m128 op2 = _mm_mul_ps (op1, op1);
      __m128 opr = _mm_movelh_ps(
        _mm_rsqrt_ps (_mm_hadd_ps (op2, _mm_undefined_ps())), 
        _mm_set1_ps(1.f)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<AngDir2>(self);
      } else return AngDir2 (opr);
    }

    template<typename Self>
    inline auto abs (this Self&& self) {
      __m128 opr = _mm_andnot_ps(_mm_set1_ps(-0.f), self.v);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self&&>(self);
      } else return AngDir2 {opr};
    }

    template<DirFin R>
    inline float pL (R&& dir) const {
      __m128 op1 = _mm_shuffle_ps(dir.v, dir.v, 0b11100001);
      __m128 opr = _mm_mul_ps (this->v, op1);
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

    template<typename Self>
    inline auto max0 (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_max_ps (self.v, _mm_setzero_ps());
        return std::forward<Self>(self);
      } else return AngDir2 (_mm_max_ps (self.v, _mm_setzero_ps()));
    }

    template<typename Self, DirFin R>
    inline auto bound (this Self&& self, R&& dir) {
      __m128 op = _mm_max_ps (_mm_min_ps (self.v, dir.v), _mm_or_ps(dir.v, _mm_set1_ps(-0.0f)));
      if constexpr (!std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>)
        op = _mm_blend_ps(op, self.v, 0b0011);

      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = op;
        return std::forward<Self>(self);
      } else if constexpr (
          std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>> &&
          std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = op;
        return std::forward<R>(dir);
      } else return AngDir2 (op);
    }

    template<typename Self>
    inline auto bound01 (this Self&& self) {
      __m128 opr = _mm_max_ps (_mm_min_ps (self.v, _mm_set1_ps(1.f)), _mm_setzero_ps());
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return AngDir2 (opr);
    }

    template<typename Self, DirFin R>
    inline auto dir_mul (this Self&& self, R&& dir) {
      __m128 opr = _mm_mul_ps (self.v, dir.v);
      if constexpr (!std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>)
        opr = _mm_blend_ps(opr, self.v, 0b0011);

      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else if constexpr (
          std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>> &&
          std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = opr;
        return std::forward<R>(dir);
      } else return AngDir2 (opr);
    }

    template<typename Self, DirFin R>
    inline auto madd (this Self&& self, const float& coef, R&& dir) {
      __m128 op = _mm_fmadd_ps (self.v, _mm_set1_ps (coef), dir.v);
      if constexpr (!std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>)
        op = _mm_blend_ps(op, self.v, 0b0011);

      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = op;
        return std::forward<Self>(self);
      } else if constexpr (
          std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>> &&
          std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = op;
        return std::forward<R>(dir);
      } else return AngDir2 (op);
    }

    template<typename Self, DirFin R>
    inline auto msub (this Self&& self, const float& coef, R&& dir) {
      __m128 op = _mm_fmsub_ps (self.v, _mm_set1_ps (coef), dir.v);
      if constexpr (!std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>)
        op = _mm_blend_ps(op, self.v, 0b0011);

      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = op;
        return std::forward<Self>(self);
      } else if constexpr (
          std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>> &&
          std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = op;
        return std::forward<R>(dir);
      } else return AngDir2 (op);
    }

    template<typename Self, DirFin R>
    inline auto nmadd (this Self&& self, const float& coef, R&& dir) {
      __m128 op = _mm_fnmadd_ps (self.v, _mm_set1_ps (coef), dir.v);
      if constexpr (!std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>)
        op = _mm_blend_ps(op, self.v, 0b0011);

      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = op;
        return std::forward<Self>(self);
      } else if constexpr (
          std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>> &&
          std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = op;
        return std::forward<R>(dir);
      } else return AngDir2 (op);
    }

    template<typename Self, DirFin R>
    inline auto nmsub (this Self&& self, const float& coef, R&& dir) {
      __m128 op = _mm_fnmsub_ps (self.v, _mm_set1_ps (coef), dir.v);
      if constexpr (!std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>)
        op = _mm_blend_ps(op, self.v, 0b0011);

      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = op;
        return std::forward<Self>(self);
      } else if constexpr (
          std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>> &&
          std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = op;
        return std::forward<R>(dir);
      } else return AngDir2 (op);
    }
};



class alignas(16) Dir3 {
  public:
    __m128 v;

    inline Dir3 () noexcept: v(_mm_setzero_ps()) {}
    inline Dir3 (__m128 m) noexcept: v(m) {}
    inline Dir3 (float x, float y, float z) noexcept { this->v = _mm_set_ps(0.f, z, y, x); }
    inline Dir3 (const Dir3 & dir) noexcept { this->v = dir.v; }
    inline Dir3 (Dir3 && dir) noexcept { this->v = dir.v; }

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
      return (_mm_movemask_ps(_mm_cmpeq_ps (this->v, dir.v)) & 0b0111) == 0b0111;
    }

    inline bool operator!= (const Dir3& dir) const {
      return (_mm_movemask_ps(_mm_cmpeq_ps (this->v, dir.v)) & 0b0111) != 0b0111;
    }

    /* Obtaining components. */
    inline float z() const {
      return std::bit_cast<float>(_mm_extract_ps(this->v, 2));
    }

    inline float y() const {
      return std::bit_cast<float>(_mm_extract_ps(this->v, 1));
    }
    
    inline float x() const {
      return _mm_cvtss_f32(this->v);
    }

    inline void z(float z) {
      __m128 z_mm = _mm_set1_ps(z);
      this->v = _mm_blend_ps(this->v, z_mm, 0b0100);
    }

    inline void y(float y) {
      __m128 y_mm = _mm_set1_ps(y);
      this->v = _mm_blend_ps(this->v, y_mm, 0b0010);
    }

    inline void x(float x) {
      __m128 x_mm = _mm_set1_ps(x);
      this->v = _mm_move_ss(this->v, x_mm);
    }

    /* Operators by overloading. */
    template<typename Self, typename R>
    requires std::is_same_v<std::remove_cvref_t<Self>, std::remove_cvref_t<R>>
    inline auto operator+ (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps (&self.x, _mm_add_ps (_mm_load_ps (&self.x), _mm_load_ps (&dir.x)));
        self.v = _mm_add_ps (dir.v, self.v);
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = _mm_add_ps (dir.v, self.v);
        return std::forward<R>(dir);
      } else return Dir3 (_mm_add_ps (dir.v, self.v));
    }

    inline Dir3& operator+= (const Dir3& dir) {
      this->v = _mm_add_ps (this->v, dir.v);
      return *this;
    }

    inline float operator* (const Dir3& dir) const {
      __m128 opr = _mm_mul_ps(this->v, dir.v);
      opr = _mm_hadd_ps(opr, opr);
      return _mm_cvtss_f32(_mm_hadd_ps(opr, opr));
    }

    template<typename Self>
    inline auto operator* (this Self&& self, const float& number) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_mul_ps (self.v, _mm_set1_ps(number));
        return std::forward<Self>(self);
      } else return Dir3 (_mm_mul_ps (self.v, _mm_set1_ps(number)));
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
      } else return Dir3 (_mm_xor_ps (self.x, _mm_set1_ps(-0.f)));
    }

    template<typename Self, typename R>
    inline auto operator- (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = _mm_sub_ps (self.v, dir.v);
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        dir.v = _mm_sub_ps (self.v, dir.v);
        return std::forward<R>(dir);
      } else return Dir3 (_mm_sub_ps (self.v, dir.v));
    }

    inline Dir3& operator-= (const Dir3& dir) {
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

    template<typename Self, typename R>
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
      } else return Dir3 (opr);
    }

    template<typename Self>
    inline auto bound01 (this Self&& self) {
      __m128 opr = _mm_max_ps (_mm_min_ps (self.v, _mm_set1_ps(1.f)), _mm_set1_ps(0.f));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        self.v = opr;
        return std::forward<Self>(self);
      } else return Dir3 (opr);
    }

    /*
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
    */
};

template<DirFin R>
MemDir2& MemDir2::store(R&& dir) {
  _mm_storel_pi((__m64*)&(this->x), dir.v);
  return *this;
}

Dir2::Dir2 (const MemDir2& dir) noexcept {
  this->v = _mm_castpd_ps(_mm_loaddup_pd((double*)&dir.x));
}

Dir2::Dir2 (const Dir3& dir3) noexcept {
  this->v = _mm_movelh_ps(dir3.v, dir3.v);
}

Dir2::Dir2 (const AngDir2& dir) noexcept {
  this->v = _mm_movelh_ps(dir.v, dir.v);
}

AngDir2::AngDir2 (const MemDir2& dir) noexcept {
  this->v = _mm_loadl_pi(_mm_setzero_ps(), (__m64*)&dir.x);
}

AngDir2::AngDir2 (const Dir2& dir) noexcept {
  this->v = _mm_movelh_ps(dir.v, _mm_setzero_ps());
}

AngDir2::AngDir2 (const Dir2& dir, const float& angle) noexcept {
  __m128 m_angle = _mm_set_ss(angle);
  this->v = _mm_insert_ps(dir.v, m_angle, 0b00100000);
}

AngDir2::AngDir2 (const Dir3& dir) noexcept {
  this->v = _mm_movelh_ps(dir.v, _mm_setzero_ps());
}

Dir3::Dir3 (const Dir2& dir) noexcept {
  this->v = _mm_movelh_ps(dir.v, _mm_setzero_ps());
}

Dir3::Dir3 (const AngDir2& dir) noexcept {
  this->v = _mm_movelh_ps(dir.v, _mm_setzero_ps());
}


/* The next if a function created to transform a compact memdir common 
 * structure into the same structure of Dir2, meant to operate with the 
 * fast implementation they provide.
 * */
template<typename F>
struct bring_memdir_type;

template<>
struct bring_memdir_type<std::pair<MemDir2, MemDir2>> {
  using type = std::pair<Dir2, Dir2>;
};

template<>
struct bring_memdir_type<std::array<MemDir2, 3>> {
  using type = std::array<Dir2, 3>;
};

template<typename F, std::size_t N, std::size_t ...I>
inline std::array<typename bring_memdir_type<F>::type, N> 
bring_memdir_aux (const std::array<F, N>& arr, std::index_sequence<I...>) {
  if constexpr (std::is_same_v<F, std::pair<MemDir2, MemDir2>>) {
    return {
      std::pair<Dir2, Dir2>{Dir2(arr[I].first), Dir2(arr[I].second)} ...
    };
  } else {
    return {
      std::array<Dir2, 3>{
        Dir2(arr[I][0]), 
        Dir2(arr[I][1]), 
        Dir2(arr[I][2])
      } ...
    };
  }
}

template<typename F, std::size_t N>
inline auto bring_memdir (const std::array<F, N>& arr) {
  return bring_memdir_aux(arr, std::make_index_sequence<N>{});
}
