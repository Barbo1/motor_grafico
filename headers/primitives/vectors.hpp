#pragma once 

#include <SDL2/SDL_cpuinfo.h>
#include <immintrin.h>
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

class alignas(8) Dir2 {
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

    /* Casting. */
    inline Dir2 (const Dir3& dir3) noexcept;
    inline Dir2 (const AngDir2& dir) noexcept;

    /* Comparations. */
    template<DirFin R>
    inline bool operator== (R&& dir) const {
      return _mm_movemask_ps(_mm_cmpeq_ps (
        _mm_loadl_pi(_mm_setzero_ps (), (__m64*)this),
        _mm_loadl_pi(_mm_setzero_ps (), (__m64*)&dir)
      )) == 0x1111;
    }

    template<DirFin R>
    inline bool operator!= (R&& dir) const {
      return _mm_movemask_ps(_mm_cmpeq_ps (
        _mm_loadl_pi(_mm_setzero_ps (), (__m64*)this),
        _mm_loadl_pi(_mm_setzero_ps (), (__m64*)&dir)
      )) != 0x1111;
    }

    /* Operators by overloading. */
    template<typename Self>
    inline auto operator- (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_epi64((__m128i_u*)&self, _mm_xor_si128(_mm_loadu_si64((uint32_t*)&self), _mm_set1_epi32(0x80000000)));
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_epi64((__m128i_u*)&ret, _mm_xor_si128(_mm_loadu_si64((uint32_t*)&self), _mm_set1_epi32(0x80000000)));
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto operator- (this Self&& self, R&& dir) {
      __m128 opr = _mm_sub_ps(
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)&self), 
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)&dir)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_pi ((__m64*)&self, opr); 
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        _mm_storel_pi ((__m64*)&dir, opr);
        return std::forward<R>(dir);
      } else {
        Dir2 ret;
        _mm_storel_pi ((__m64*)&ret, opr);
        return ret;
      }
    }

    template<DirFin R>
    inline Dir2& operator-= (R&& dir) {
      _mm_storel_pi ((__m64*)this, _mm_sub_ps(
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)this), 
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)&dir)
      )); 
      return *this;
    }
    
    template<typename Self, DirFin R>
    inline auto operator+ (this Self&& self, R&& dir) {
      __m128 opr = _mm_add_ps(
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)&self), 
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)&dir)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_pi ((__m64*)&self, opr);
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        _mm_storel_pi ((__m64*)&dir, opr);
        return std::forward<R>(dir);
      } else { 
        Dir2 ret;
        _mm_storel_pi ((__m64*)&ret, opr);
        return ret;
      }
    }

    template<DirFin R> 
    inline Dir2& operator+= (R&& d) {
      _mm_storel_pi ((__m64*)this, _mm_add_ps(
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)this), 
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)&d)
      ));
      return *this;
    }
    
    template<DirFin R>
    inline float operator* (R&& dir) const {
      __m128 res = _mm_mul_ps(
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)this), 
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)&dir)
      );
      return _mm_cvtss_f32(_mm_hadd_ps (res, res));
    }

    template<typename Self>
    inline auto operator* (this Self&& self, const float& number) {
      __m128 opr = _mm_mul_ps(
        _mm_loadl_pi (_mm_undefined_ps(), (__m64*)&self), 
        _mm_set1_ps (number) 
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_pi ((__m64*)&self, opr);
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_pi ((__m64*)&ret, opr);
        return ret;
      }
    }

    template<typename Self>
    inline auto operator/ (this Self&& self, const float& number) {
      __m128 opr = _mm_mul_ps(
        _mm_loadl_pi (_mm_undefined_ps(), (__m64*)&self), 
        _mm_rcp_ps(_mm_set1_ps (number))
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_pi ((__m64*)&self, opr);
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_pi ((__m64*)&ret, opr);
        return ret;
      }
    }

    inline Dir2& operator*= (const float& number) {
      _mm_storel_pi ((__m64*)this, _mm_mul_ps(
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)this), 
        _mm_set1_ps(number)
      ));
      return *this;
    }

    /* Operations by functions. */
    template<typename Self>
    inline auto percan (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>){
        _mm_storel_epi64((__m128i_u*)&self, _mm_xor_si128(
          _mm_shuffle_epi32(_mm_loadl_epi64((__m128i_u*)&self), 0b00000001),
          _mm_set_epi64x(0, 0x80000000)
        ));
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_epi64((__m128i_u*)&ret,_mm_xor_si128(
          _mm_shuffle_epi32(_mm_loadl_epi64((__m128i_u*)&self), 0b00000001),
          _mm_set_epi64x(0, 0x80000000)
        ));
        return ret;
      }
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
      __m128 charged = _mm_loadl_pi (_mm_undefined_ps(), (__m64*)&self);
      __m128 opr = _mm_mul_ps (charged, charged);
      opr = _mm_mul_ps (_mm_set1_ps(_mm_cvtss_f32(_mm_rsqrt_ss(_mm_hadd_ps(opr, opr)))), charged);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_pi ((__m64*)&self, opr);
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_pi ((__m64*)&ret, opr);
        return ret;
      }
    }

    template<typename Self>
    inline auto abs (this Self&& self) {
      __m128i opr = _mm_and_si128(_mm_loadu_si64((uint32_t*)&self), _mm_set1_epi32(0x7FFFFFFF));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_epi64((__m128i_u*)&self, opr);
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_epi64((__m128i_u*)&ret, opr);
        return ret;
      }
    }

    template<DirFin R>
    inline float pL (R&& dir) const {
      __m128 opr = _mm_mul_ps (
        _mm_castsi128_ps (_mm_shuffle_epi32(_mm_loadl_epi64((__m128i_u*)this), 0b00000001)),
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)&dir)
      );
      return _mm_cvtss_f32 (_mm_hsub_ps(opr, opr));
    }

    inline float modulo () const {
      __m128 charged = _mm_loadl_pi (_mm_undefined_ps(), (__m64*)this);
      __m128 opr = _mm_mul_ps(charged, charged);
      return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(opr, opr)));
    }

    inline float modulo2 () const {
      __m128 charged = _mm_loadl_pi (_mm_undefined_ps(), (__m64*)this);
      __m128 opr = _mm_mul_ps(charged, charged);
      return _mm_cvtss_f32(_mm_hadd_ps(opr, opr));
    }

    inline float angle () const {
      __m128 charged = _mm_loadl_pi (_mm_undefined_ps(), (__m64*)this);
      __m128 opr = _mm_mul_ps(charged, charged);
      return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_hadd_ps(opr, opr))) * _mm_cvtss_f32(_mm_hadd_ps(charged, charged));
    }

    template<typename Self>
    inline auto max0 (this Self&& self) {
      __m128 opr = _mm_max_ps (_mm_loadl_pi (_mm_undefined_ps(), (__m64*)&self), _mm_set1_ps(0.f));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_pi((__m64*)&self.x, opr);
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_pi((__m64*)&ret.x, opr);
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto bound (this Self&& self, R&& dir) {
      __m128 other = _mm_loadl_pi (_mm_undefined_ps(), (__m64*)&dir);
      __m128 opr = _mm_min_ps (_mm_loadl_pi (_mm_undefined_ps(), (__m64*)&self), other);
      opr = _mm_max_ps (opr, _mm_or_ps(other, _mm_set1_ps(-0.0f)));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_pi((__m64*)&self.x, opr);
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_pi((__m64*)&ret.x, opr);
        return ret;
      }
    }

    template<typename Self>
    inline auto bound01 (this Self&& self) {
      __m128 opr = _mm_max_ps (
        _mm_min_ps (
          _mm_loadl_pi (_mm_undefined_ps(), (__m64*)&self), 
          _mm_set1_ps(1.f)
        ), 
        _mm_set1_ps(0.f)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_pi((__m64*)&self.x, opr);
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_pi((__m64*)&ret.x, opr);
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto dir_mul (this Self&& self, R&& dir) {
      __m128 opr = _mm_mul_ps (
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&self.x), 
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_pi((__m64*)&self.x, opr);
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_pi((__m64*)&ret.x, opr);
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto madd (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fmadd_ps (
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&self.x),
        _mm_set1_ps (coef),
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_pi((__m64*)&self.x, opr);
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_pi((__m64*)&ret.x, opr);
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto msub (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fmsub_ps (
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&self.x),
        _mm_set1_ps (coef),
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_pi((__m64*)&self.x, opr);
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_pi((__m64*)&ret.x, opr);
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto nmadd (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fnmadd_ps (
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&self.x),
        _mm_set1_ps (coef),
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_pi((__m64*)&self.x, opr);
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_pi((__m64*)&ret.x, opr);
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto nmsub (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fnmsub_ps (
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&self.x),
        _mm_set1_ps (coef),
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_storel_pi((__m64*)&self.x, opr);
        return std::forward<Self>(self);
      } else {
        Dir2 ret;
        _mm_storel_pi((__m64*)&ret.x, opr);
        return ret;
      }
    }
};



class alignas(16) AngDir2 {
  public:
    float x;
    float y;
    float a;
    float pad;
    
    /* Basic operations. */
    inline AngDir2 () noexcept  {
      _mm_store_ps(&this->x, _mm_set_ps(0.f, 0.f, 0.f, 0.f));
    }

    inline AngDir2 (float coef) noexcept {
      _mm_store_ps(&this->x, _mm_set_ps(0.f, coef, coef, coef));
    }

    inline AngDir2 (float x, float y, float a) noexcept {
      _mm_store_ps(&this->x, _mm_set_ps(0.f, a, y, x));
    }

    inline AngDir2 (const AngDir2 & adir) noexcept {
      _mm_store_ps(&this->x, _mm_set_ps(0.f, adir.a, adir.y, adir.x));
    }

    inline AngDir2 & operator= (const AngDir2 & adir) noexcept {
      _mm_store_ps ((float*)this, _mm_load_ps (&adir.x));
      return *this;
    }

    /* Casting. */
    inline AngDir2 (const Dir2& dir) noexcept;
    inline AngDir2 (const Dir3& dir) noexcept;

    /* Comparations. */
    template<DirFin R>
    inline bool operator== (R&& adir) const {
      if constexpr (std::is_same_v<R, AngDir2>) 
        return _mm_movemask_ps(_mm_cmpeq_ps (
          _mm_load_ps((float*)this),
          _mm_load_ps((float*)&adir)
        )) == 0x1111;
      else 
        return _mm_movemask_ps(_mm_cmpeq_ps (
          _mm_loadl_pi(_mm_setzero_ps (), (__m64*)this),
          _mm_loadl_pi(_mm_setzero_ps (), (__m64*)&adir)
        )) == 0x1111;
    }

    template<DirFin R>
    inline bool operator!= (R&& adir) const {
      if constexpr (std::is_same_v<R, AngDir2>) 
        return _mm_movemask_ps(_mm_cmpeq_ps (
          _mm_load_ps((float*)this),
          _mm_load_ps((float*)&adir)
        )) != 0x1111;
      else 
        return _mm_movemask_ps(_mm_cmpeq_ps (
          _mm_loadl_pi(_mm_setzero_ps (), (__m64*)this),
          _mm_loadl_pi(_mm_setzero_ps (), (__m64*)&adir)
        )) != 0x1111;
    }

    /* Operators by overloading. */
    template<typename Self, DirFin R>
    inline auto operator+ (this Self&& self, R&& dir) {
      if constexpr (std::is_same_v<std::remove_cvref_t<R>, AngDir2>) {
        if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
          _mm_store_ps (&self.x, _mm_add_ps (_mm_load_ps (&self.x), _mm_load_ps (&dir.x)));
          return std::forward<Self>(self);
        } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
          _mm_store_ps (&dir.x, _mm_add_ps (_mm_load_ps (&self.x), _mm_load_ps (&dir.x)));
          return std::forward<R>(dir);
        } else {
          AngDir2 ret;
          _mm_store_ps (&ret.x, _mm_add_ps (_mm_load_ps (&self.x), _mm_load_ps (&dir.x)));
          return ret;
        }
      } else {
        if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
          _mm_storel_pi ((__m64*)&self.x, _mm_add_ps (
            _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&self.x), 
            _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
          ));
          return std::forward<Self>(self);
        } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
          _mm_storel_pi ((__m64*)&dir.x, _mm_add_ps (
            _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&self.x), 
            _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
          ));
          return std::forward<R>(dir);
        } else {
          Dir2 ret;
          _mm_storel_pi ((__m64*)&ret.x, _mm_add_ps (
            _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&self.x), 
            _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
          ));
          return ret;
        }
      }
    }

    template<DirFin R> 
    inline AngDir2& operator+= (R&& dir) {
      if constexpr (std::is_same_v<std::remove_cvref_t<R>, AngDir2>) {
        _mm_store_ps (&this->x, _mm_add_ps (_mm_load_ps (&this->x), _mm_load_ps (&dir.x)));
      } else {
        _mm_storel_pi ((__m64*)&this->x, _mm_add_ps (
          _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&this->x), 
          _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
        ));
      }
      return *this;
    }

    template<DirFin R>
    inline float operator* (R&& dir) const {
      __m128 res = _mm_mul_ps(
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)this), 
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)&dir)
      );
      return _mm_cvtss_f32(_mm_hadd_ps (res, res));
    }

    template<typename Self>
    inline auto operator* (this Self&& self, const float& number) {
      __m128 opr = _mm_mul_ps(
        _mm_load_ps (&self.x), 
        _mm_set_ps (0.f, 1.f, number, number)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps (&self.x, opr);
        return std::forward<Self>(self);
      } else {
        AngDir2 ret;
        _mm_store_ps (&ret.x, opr);
        return ret;
      }
    }

    template<typename Self>
    inline auto operator/ (this Self&& self, const float& number) {
      __m128 opr = _mm_mul_ps(
        _mm_load_ps (&self.x), 
        _mm_rcp_ps(_mm_set_ps (1.f, 1.f, number, number))
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps (&self.x, opr);
        return std::forward<Self>(self);
      } else {
        AngDir2 ret;
        _mm_store_ps (&ret.x, opr);
        return ret;
      }
    }

    inline AngDir2& operator*= (const float& number) {
      _mm_store_ps (&this->x, _mm_mul_ps(
        _mm_load_ps (&this->x), 
        _mm_set_ps (0.f, 1.f, number, number)
      ));
      return *this;
    }

    template<typename Self>
    inline auto operator- (this Self&& self) {
      __m128 opr = _mm_xor_ps(
        _mm_load_ps((float*)&self.x), 
        _mm_castsi128_ps(_mm_set1_epi32(0x80000000))
      );
      if (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps((float*)&self.x, opr);
        return std::forward<Self>(self);
      } else {
        AngDir2 ret;
        _mm_store_ps((float*)&ret.x, opr);
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto operator- (this Self&& self, R&& dir) {
      if constexpr (std::is_same_v<std::remove_cvref_t<R>, AngDir2>) {
        __m128 opr = _mm_sub_ps (_mm_load_ps (&self.x), _mm_load_ps (&dir.x));
        if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
          _mm_store_ps (&self.x, opr);
          return std::forward<Self>(self);
        } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
          _mm_store_ps (&dir.x, opr);
          return std::forward<R>(dir);
        } else {
          AngDir2 ret;
          _mm_store_ps (&ret.x, opr);
          return ret;
        }
      } else {
        __m128 opr = _mm_sub_ps (
          _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&self.x), 
          _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
        );
        if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
          _mm_storel_pi ((__m64*)&self.x, opr);
          return std::forward<Self>(self);
        } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
          _mm_storel_pi ((__m64*)&dir.x, opr);
          return std::forward<R>(dir);
        } else {
          Dir2 ret;
          _mm_storel_pi ((__m64*)&ret.x, opr);
          return ret;
        }
      }
    }

    template<DirFin R>
    inline AngDir2& operator-= (R&& dir) {
      _mm_store_ps (&this->x, _mm_sub_ps (_mm_load_ps (&this->x), _mm_load_ps (&dir.x)));
      return *this;
    }

    /* Operations by functions. */
    template<typename Self>
    inline auto percan (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>){
        _mm_storel_epi64((__m128i_u*)&self, _mm_xor_si128(
          _mm_shuffle_epi32(_mm_loadl_epi64((__m128i_u*)&self), 0b00000001),
          _mm_set_epi64x(0, 0x80000000)
        ));
        return std::forward<Self>(self);
      } else {
        AngDir2 ret;
        _mm_storel_epi64((__m128i_u*)&ret,_mm_xor_si128(
          _mm_shuffle_epi32(_mm_loadl_epi64((__m128i_u*)&self), 0b00000001),
          _mm_set_epi64x(0, 0x80000000)
        ));
        return ret;
      }
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
      __m128 charged = _mm_load_ps (&self.x);
      __m128 opr = _mm_mul_ps (charged, charged);
      float b = _mm_cvtss_f32(_mm_rsqrt_ss(_mm_hadd_ps(opr, opr)));
      opr = _mm_mul_ps(_mm_set_ps (0.f, 1.f, b, b), charged);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps(&self.x, opr);
        return std::forward<AngDir2>(self);
      } else {
        AngDir2 ret;
        _mm_store_ps(&ret.x, opr);
        return ret;
      }
    }

    template<typename Self>
    inline auto abs (this Self&& self) {
      __m128i opr = _mm_and_si128(
        _mm_load_si128 ((__m128i*)&self), 
        _mm_set1_epi32(0x7FFFFFFF)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_si128 ((__m128i*)&self, opr);
        return std::forward<Self&&>(self);
      } else {
        AngDir2 ret;
        _mm_store_si128 ((__m128i*)&ret, opr);
        return ret;
      }
    }

    template<DirFin R>
    inline float pL (R&& dir) const {
      __m128 opr = _mm_mul_ps (
        _mm_castsi128_ps (_mm_shuffle_epi32(_mm_loadl_epi64((__m128i_u*)this), 0b00000001)),
        _mm_loadl_pi(_mm_undefined_ps(), (__m64*)&dir)
      );
      return _mm_cvtss_f32 (_mm_hsub_ps(opr, opr));
    }

    inline float modulo () const {
      __m128 charged = _mm_loadl_pi (_mm_undefined_ps(), (__m64*)this);
      __m128 opr = _mm_mul_ps(charged, charged);
      return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(opr, opr)));
    }

    inline float modulo2 () const {
      __m128 charged = _mm_loadl_pi (_mm_undefined_ps(), (__m64*)this);
      __m128 opr = _mm_mul_ps(charged, charged);
      return _mm_cvtss_f32(_mm_hadd_ps(opr, opr));
    }

    inline float angle () const {
      __m128 charged = _mm_loadl_pi (_mm_undefined_ps(), (__m64*)this);
      __m128 opr = _mm_mul_ps(charged, charged);
      return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_hadd_ps(opr, opr))) * _mm_cvtss_f32(_mm_hadd_ps(charged, charged));
    }

    template<typename Self>
    inline auto max0 (this Self&& self) {
      __m128 opr = _mm_max_ps (_mm_load_ps (&self.x), _mm_set1_ps(0.f));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps(&self.x, opr);
        return std::forward<Self>(self);
      } else {
        AngDir2 ret;
        _mm_store_ps(&ret.x, opr);
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto bound (this Self&& self, R&& dir) {
      __m128 other = _mm_load_ps (&dir.x);
      __m128 opr = _mm_min_ps (_mm_load_ps (&self.x), other);
      opr = _mm_max_ps (opr, _mm_or_ps(other, _mm_set1_ps(-0.0f)));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps(&self.x, opr);
        return std::forward<Self>(self);
      } else {
        AngDir2 ret;
        _mm_store_ps(&ret.x, opr);
        return ret;
      }
    }

    template<typename Self>
    inline auto bound01 (this Self&& self) {
      __m128 opr = _mm_max_ps (_mm_min_ps (_mm_load_ps (&self.x), _mm_set1_ps(1.f)), _mm_set1_ps(0.f));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps(&self.x, opr);
        return std::forward<Self>(self);
      } else {
        AngDir2 ret;
        _mm_store_ps(&ret.x, opr);
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto dir_mul (this Self&& self, R&& dir) {
      __m128 opr = _mm_mul_ps (_mm_load_ps (&self.x), _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x));
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps(&self.x, opr);
        return std::forward<Self>(self);
      } else {
        AngDir2 ret;
        _mm_store_ps(&ret.x, opr);
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto madd (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fmadd_ps (
        _mm_load_ps (&self.x), 
        _mm_set1_ps (coef),
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps(&self.x, opr);
        return std::forward<Self>(self);
      } else {
        AngDir2 ret;
        _mm_store_ps(&ret.x, opr);
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto msub (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fmsub_ps (
        _mm_load_ps (&self.x), 
        _mm_set1_ps (coef),
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps(&self.x, opr);
        return std::forward<Self>(self);
      } else {
        AngDir2 ret;
        _mm_store_ps(&ret.x, opr);
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto nmadd (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fnmadd_ps (
        _mm_load_ps (&self.x), 
        _mm_set1_ps (coef),
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps(&self.x, opr);
        return std::forward<Self>(self);
      } else {
        AngDir2 ret;
        _mm_store_ps(&ret.x, opr);
        return ret;
      }
    }

    template<typename Self, DirFin R>
    inline auto nmsub (this Self&& self, const float& coef, R&& dir) {
      __m128 opr = _mm_fnmsub_ps (
        _mm_load_ps (&self.x), 
        _mm_set1_ps (coef),
        _mm_loadl_pi (_mm_setzero_ps(), (__m64*)&dir.x)
      );
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps(&self.x, opr);
        return std::forward<Self>(self);
      } else {
        AngDir2 ret;
        _mm_store_ps(&ret.x, opr);
        return ret;
      }
    }
};



class alignas(16) Dir3 {
  public:
    float x;
    float y;
    float z;
    float pad;

    inline Dir3 () noexcept: x(0.f), y(0.f), z(0.f), pad(0.f) {}
    inline Dir3 (float x, float y, float z) noexcept: x(x), y(y), z(z), pad(0.f) {}
    inline Dir3 (const Dir3 & dir) noexcept: x(dir.x), y(dir.y), z(dir.z), pad(0.f) {}
    inline Dir3 (Dir3 && dir) noexcept: x(dir.x), y(dir.y), z(dir.z), pad(0.f) {}

    inline Dir3& operator= (const Dir3 & dir) noexcept {
      _mm_store_ps ((float*)this, _mm_load_ps (&dir.x));
      return *this;
    }

    inline Dir3& operator= (Dir3 && dir) noexcept {
      _mm_store_ps ((float*)this, _mm_load_ps (&dir.x));
      return *this;
    }

    /* Casting. */
    inline Dir3 (const Dir2& dir) noexcept;
    inline Dir3 (const AngDir2& dir) noexcept;

    /* Comparations. */
    inline bool operator== (const Dir3& dir) const {
      return _mm_movemask_ps(_mm_cmpeq_ps (
        _mm_load_ps(&this->x),
        _mm_load_ps(&dir.x)
      )) == 0x1111;
    }

    inline bool operator!= (const Dir3& dir) const {
      return _mm_movemask_ps(_mm_cmpeq_ps (
        _mm_load_ps(&this->x),
        _mm_load_ps(&dir.x)
      )) != 0x1111;
    }

    /* Operators by overloading. */
    template<typename Self, DirFin3 R>
    inline auto operator+ (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps (&self.x, _mm_add_ps (_mm_load_ps (&self.x), _mm_load_ps (&dir.x)));
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        _mm_store_ps (&dir.x, _mm_add_ps (_mm_load_ps (&self.x), _mm_load_ps (&dir.x)));
        return std::forward<R>(dir);
      } else {
        AngDir2 ret;
        _mm_store_ps (&ret.x, _mm_add_ps (_mm_load_ps (&self.x), _mm_load_ps (&dir.x)));
        return ret;
      }
    }

    template<DirFin3 R> 
    inline Dir3& operator+= (R&& dir) {
      _mm_store_ps (&this->x, _mm_add_ps (_mm_load_ps (&this->x), _mm_load_ps (&dir.x)));
      return *this;
    }

    template<DirFin3 R>
    inline float operator* (R&& dir) const {
      __m128 opr = _mm_mul_ps(_mm_load_ps(&this->x), _mm_load_ps(&dir.x));
      opr = _mm_hadd_ps(opr, opr);
      return _mm_cvtss_f32(_mm_hadd_ps(opr, opr));
    }

    template<typename Self>
    inline auto operator* (this Self&& self, const float& number) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps (&self.x, _mm_mul_ps (_mm_load_ps (&self.x), _mm_set1_ps(number)));
        return std::forward<Self>(self);
      } else {
        Dir3 ret;
        _mm_store_ps (&ret.x, _mm_mul_ps (_mm_load_ps (&self.x), _mm_set1_ps(number)));
        return ret;
      }
    }

    inline Dir3& operator*= (const float& number) {
      _mm_store_ps (&this->x, _mm_mul_ps (_mm_load_ps (&this->x), _mm_set1_ps(number)));
      return *this;
    }

    template<typename Self>
    inline auto operator- (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps (&self.x, _mm_xor_ps (_mm_load_si128 (&self.x), _mm_set1_epi32(0x80000000)));
        return std::forward<Self>(self);
      } else {
        Dir3 ret;
        _mm_store_ps (&ret.x, _mm_xor_ps (_mm_load_si128 (&self.x), _mm_set1_epi32(0x80000000)));
        return ret;
      }
    }

    template<typename Self, DirFin3 R>
    inline auto operator- (this Self&& self, R&& dir) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps (&self.x, _mm_sub_ps (_mm_load_ps (&self.x), _mm_load_ps (&dir.x)));
        return std::forward<Self>(self);
      } else if constexpr (std::is_rvalue_reference_v<R&&> && !std::is_const_v<R&&>) {
        _mm_store_ps (&dir.x, _mm_sub_ps (_mm_load_ps (&self.x), _mm_load_ps (&dir.x)));
        return std::forward<R>(dir);
      } else {
        AngDir2 ret;
        _mm_store_ps (&ret.x, _mm_sub_ps (_mm_load_ps (&self.x), _mm_load_ps (&dir.x)));
        return ret;
      }
    }

    template<DirFin3 R>
    inline Dir3& operator-= (R&& dir) {
      _mm_store_ps (&this->x, _mm_sub_ps (_mm_load_ps (&this->x), _mm_load_ps (&dir.x)));
      return *this;
    }

    /* Operations by functions. */

    template<typename Self>
    inline auto abs (this Self&& self) {
      if constexpr (std::is_rvalue_reference_v<Self&&> && ! std::is_const_v<Self&&>) {
        _mm_store_si128((__m128i_u*)&self, _mm_and_si128(_mm_load_si128(&self), _mm_set1_epi32(0x7FFFFFFF)));
        return std::forward<Self>(self);
      } else {
        Dir3 ret;
        _mm_store_si128((__m128i_u*)&ret, _mm_and_si128(_mm_load_si128(&self), _mm_set1_epi32(0x7FFFFFFF)));
        return ret;
      }
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
      __m128 opr = _mm_load_ps(&this->x);
      opr = _mm_mul_ps(opr, opr);
      opr = _mm_hadd_ps(opr, opr);
      return _mm_cvtss_f32(_mm_sqrt_ss(_mm_hadd_ps(opr, opr)));
    }

    inline float modulo2 () const {
      __m128 opr = _mm_load_ps(&this->x);
      opr = _mm_mul_ps(opr, opr);
      opr = _mm_hadd_ps(opr, opr);
      return _mm_cvtss_f32(_mm_hadd_ps(opr, opr));
    }

    template<typename Self>
    inline auto normalize (this Self&& self) {
      __m128 charged = _mm_load_ps (&self.x);
      __m128 opr = _mm_mul_ps (charged, charged);
      opr = _mm_hadd_ps(opr, opr);
      float b = _mm_cvtss_f32(_mm_rsqrt_ss(_mm_hadd_ps(opr, opr)));
      opr = _mm_mul_ps(_mm_set1_ps (b), charged);
      if constexpr (std::is_rvalue_reference_v<Self&&> && !std::is_const_v<Self&&>) {
        _mm_store_ps (&self.x, opr);
        return std::forward<Self> (self);
      } else {
        Dir3 ret;
        _mm_store_ps (&ret.x, opr);
        return ret;
      }
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
