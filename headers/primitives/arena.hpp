#pragma once

#include "./types_definition.hpp"
#include <cstddef>
#include <vector>

struct ArenaConstexFlag {
  std::byte* context_current;
  std::size_t context_remaining;
};

class Arena {
  private:
    std::byte* init_ptr;
    std::byte* curr_ptr;
    std::size_t size, remaining;
  public:
    Arena(std::size_t size);
    ~Arena();

    std::byte* allocd(std::size_t size, std::size_t alignment = 1);
    std::byte* allocp(std::size_t size, std::size_t pot = 0);

    template<typename T> T* talloc(std::size_t size) {
      return (T*)(this->allocp(size * sizeof(T)));
    }

    template<typename T> T* atalloc(std::size_t size) {
      return (T*)(this->allocd(size * sizeof(T), alignof(T)));
    }

    ArenaConstexFlag get_context();
    void go_back_context(ArenaConstexFlag);

    void restart();
};

class DynamicalArena {
  private:
    std::byte* curr_ptr;
    std::vector<std::byte*> ptr_blocks;
    std::size_t size, remaining;

    // pointers to free memory.
    MaskObject* freed_mo;
  public:
    DynamicalArena(std::size_t size);
    ~DynamicalArena();
  
    MaskObject* alloc_mo ();
    void free_mo (MaskObject*);
    void complete_free_mo (MaskObject*);
};

/*
template<typename T>
class ListDA {
  private:
    std::size_t n;
    T* elems;
    DynamicalArena* darena;

  public:
    ListDA(DynamicalArena& arena);
    ListDA(const ListDA&);
    ListDA(ListDA&&);
    ListDA operator=(const ListDA&);
    ListDA operator=(ListDA&&);

    template<T> void push_back(T);
    template<T> T pop_back();
    template<T> T back();
    
    std::size_t size();

    
};
*/
