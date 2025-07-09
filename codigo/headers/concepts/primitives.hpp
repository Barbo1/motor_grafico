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

enum AngularType {
  AT_CONSTANT,
  AT_TIME,
  AT_POSITION,
  AT_VELOCITY,
};

/* ------------------------------ */
/* Low level data representation. */
/* ------------------------------ */

class Direction;
class AngularDirection;

template<class T>
concept DirFin = std::is_same_v<T, AngularDirection> || std::is_same_v<T, Direction>;

class Direction {
  public:
    float x;
    float y;

    Direction ();
    Direction (float, float);
    Direction (const Direction &);
    Direction (Direction &&);
    Direction & operator= (const Direction &);
    Direction & operator= (Direction &&);
    bool operator== (const Direction &);

    Direction operator- ();

    template<DirFin R> Direction operator+ (const R &);
    template<DirFin R> Direction operator- (const R &);
    Direction operator* (float);
    template<DirFin R> float operator* (const R &);
    template<DirFin R> void operator+= (const R &);
    template<DirFin R> void operator-= (const R &);
    virtual void operator*= (float);
    virtual Direction normalize ();
    virtual float modulo ();
};

class AngularDirection: public Direction {
  public:
    float a;
    
    AngularDirection ();
    AngularDirection (float, float, float);
    AngularDirection (const AngularDirection &);
    AngularDirection (AngularDirection &&);
    AngularDirection & operator= (const AngularDirection &);
    AngularDirection & operator= (AngularDirection &&);
    bool operator== (const AngularDirection &);

    AngularDirection operator- ();

    template<DirFin R> AngularDirection operator+ (R);
    template<DirFin R> AngularDirection operator- (R);
    AngularDirection operator* (float f);
    template<DirFin R> float operator* (R);
    template<DirFin R> void operator+= (R);
    template<DirFin R> void operator-= (R);
};
