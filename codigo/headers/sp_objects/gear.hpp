#pragma once 

#include "../concepts/physical.hpp"
#include <vector>

class Gear: private Physical {
  private: 
    std::vector<Physical*> objects;
    AngDir2 position;
    AngDir2 velocity;
    AngDir2 force;
    
  public:
    Gear () noexcept;
    Gear (const Gear &) noexcept;
    Gear (Gear &&) noexcept;
    Gear & operator= (const Gear &) noexcept;
    Gear & operator= (Gear &&) noexcept;
    ~Gear () = default;

    void add_obj (Physical *);

    void set_fixed (bool);
    bool get_fixed ();
};
