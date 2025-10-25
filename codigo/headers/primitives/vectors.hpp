#pragma once 

#include <type_traits>

enum Function {
  FT_CONSTANT,
  FT_LINEAR,
  FT_QUADRATIC,
  FT_CUBIC,
  FT_EXPONENTIAL,
  FT_SINUSOIDAL,
};

enum AngType {
  AT_TIME,
  AT_POSITION,
  AT_VELOCITY,
};

/* ------------------------------ */
/* Low level data representation. */
/* ------------------------------ */

class Dir2;
class Dir3;
class AngDir2;

template<class T>
concept DirFin = std::is_same_v<T, AngDir2> || std::is_same_v<T, Dir2>;

class Dir2 {
  public:
    float x;
    float y;

    Dir2 ();
    Dir2 (Dir3);
    Dir2 (float, float);
    Dir2 (const Dir2 &);
    Dir2 (Dir2 &&);
    Dir2 & operator= (const Dir2 &);
    Dir2 & operator= (Dir2 &&);
    bool operator== (const Dir2 &);
    bool operator!= (const Dir2 &);

    Dir2 operator- ();
    Dir2 operator* (float) const;

    void rotate (float angle);

    template<DirFin R> Dir2 operator+ (const R &) const;
    template<DirFin R> Dir2 operator- (const R &) const;
    template<DirFin R> float operator* (const R &) const;
    template<DirFin R> void operator+= (const R &);
    template<DirFin R> void operator-= (const R &);

    virtual void operator*= (float);
    virtual Dir2 abs ();
    virtual float modulo () const;
    virtual float modulo2 () const;
    Dir2 normalize () const;
    Dir2 percan() const;
};

class Dir3 {
  public:
    float x;
    float y;
    float z;

    Dir3 ();
    Dir3 (float, float, float);
    Dir3 (const Dir3 &);
    Dir3 (Dir3 &&);
    Dir3 & operator= (const Dir3 &);
    Dir3 & operator= (Dir3 &&);
    bool operator== (const Dir3 &);
    bool operator!= (const Dir3 &);

    Dir3 operator- () const;

    Dir3 operator+ (const Dir3 &) const;
    Dir3 operator- (const Dir3 &) const;
    Dir3 operator* (float) const;
    float operator* (const Dir3 &) const;
    void operator+= (const Dir3 &);
    void operator-= (const Dir3 &);
    void operator*= (float);
    Dir3 cross (const Dir3&) const;

    Dir3 abs ();
    float modulo () const;
    float modulo2 () const;
    Dir3 normalize () const;

    void rotate_x (float angle);
    void rotate_y (float angle);
    void rotate_z (float angle);
};

class AngDir2: public Dir2 {
  public:
    float a;
    
    AngDir2 ();
    AngDir2 (float, float, float);
    AngDir2 (const AngDir2 &);
    AngDir2 (AngDir2 &&);
    AngDir2& operator= (const AngDir2 &);
    AngDir2& operator= (AngDir2 &&);
    bool operator== (const AngDir2 &);

    AngDir2 operator- () const;
    AngDir2 operator* (float f) const;
    float angle () const;
    AngDir2 normalize ();

    template<DirFin R> AngDir2 operator+ (R) const;
    template<DirFin R> AngDir2 operator- (R) const;
    template<DirFin R> float operator* (R) const;
    template<DirFin R> void operator+= (R);
    template<DirFin R> void operator-= (R);
};
