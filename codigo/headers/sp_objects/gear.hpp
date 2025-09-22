#pragma once 

#include "../concepts/physical.hpp"

class Gear: private Physical {
  private: 
    std::vector<Physical*> objects;
    AngDir2 position;
    AngDir2 velocity;
    AngDir2 force;
    
  public:
    Gear ();
    Gear (const Gear &);
    Gear (Gear &&);
    Gear & operator= (const Gear &);
    Gear & operator= (Gear &&);
    ~Gear ();

    void add_obj (Physical *);

    void set_fixed (bool);
    bool get_fixed ();
};
