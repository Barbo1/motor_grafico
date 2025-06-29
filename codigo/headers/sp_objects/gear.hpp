#pragma once 

#include "../pr_concepts/physical.hpp"

class Gear: private Physical {
  private: 
    std::vector<Physical*> objects;
    bool fixed;
    
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
